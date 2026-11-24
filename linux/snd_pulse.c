#include <stdlib.h>
#include <time.h>
#include <pulse/pulseaudio.h>

#include "../client/client.h"
#include "../client/snd_loc.h"

#define PULSE_RING_FRAMES 65536

static pa_mainloop *pulse_loop;
static pa_context *pulse_context;
static pa_stream *pulse_stream;
static uint64 pulse_start;
static uint64 pulse_next_frame;
static size_t pulse_frame_bytes;
static qboolean pulse_failed;
static qboolean pulse_queued;

static uint64 Pulse_Nanoseconds(void)
{
	struct timespec now;

	clock_gettime(CLOCK_MONOTONIC, &now);
	return (uint64)now.tv_sec * 1000000000ULL + now.tv_nsec;
}

static uint64 Pulse_FrameTime(void)
{
	uint64 elapsed = Pulse_Nanoseconds() - pulse_start;

	return (elapsed / 1000000000ULL) * dma.speed
		+ (elapsed % 1000000000ULL) * dma.speed / 1000000000ULL;
}

static void Pulse_Error(const char *operation)
{
	if (!pulse_failed)
		Com_Printf("PulseAudio %s: %s\n", LOG_CLIENT, operation,
			pulse_context ? pa_strerror(pa_context_errno(pulse_context)) : "mainloop unavailable");
	pulse_failed = true;
}

static qboolean Pulse_Pump(void)
{
	int i;

	if (pulse_failed)
		return false;

	for (i = 0; i < 32; i++)
	{
		int result = pa_mainloop_iterate(pulse_loop, 0, NULL);

		if (result < 0)
		{
			Pulse_Error("mainloop failed");
			return false;
		}
		if (!result)
			break;
	}
	if (!PA_CONTEXT_IS_GOOD(pa_context_get_state(pulse_context)) ||
		(pulse_stream && !PA_STREAM_IS_GOOD(pa_stream_get_state(pulse_stream))))
	{
		Pulse_Error("connection lost");
		return false;
	}
	return true;
}

static qboolean Pulse_WaitReady(void)
{
	const struct timespec delay = { 0, 1000000 };
	uint64 deadline = Pulse_Nanoseconds() + 2000000000ULL;

	do
	{
		if (!Pulse_Pump())
			return false;
		if (pulse_stream ? pa_stream_get_state(pulse_stream) == PA_STREAM_READY
			: pa_context_get_state(pulse_context) == PA_CONTEXT_READY)
			return true;
		nanosleep(&delay, NULL);
	} while (Pulse_Nanoseconds() < deadline);

	Com_Printf("PulseAudio connection timed out.\n", LOG_CLIENT);
	pulse_failed = true;
	return false;
}

static void Pulse_Clear(void)
{
	pa_operation *operation;

	if (!Pulse_Pump() || !pulse_queued)
		return;
	operation = pa_stream_flush(pulse_stream, NULL, NULL);
	if (!operation)
	{
		Pulse_Error("flush failed");
		return;
	}
	pa_operation_unref(operation);
	pulse_queued = false;
	pulse_next_frame = Pulse_FrameTime();
}

static void Pulse_Submit(uint64 first_frame, unsigned frames)
{
	uint64 end_frame = first_frame + frames;
	size_t writable;

	if (!Pulse_Pump())
		return;

	writable = pa_stream_writable_size(pulse_stream);
	if (writable == (size_t)-1)
	{
		Pulse_Error("buffer query failed");
		return;
	}
	writable /= pulse_frame_bytes;

	// The mixer has advanced past any frames before first_frame.
	if (pulse_next_frame < first_frame)
		pulse_next_frame = first_frame;

	while (writable && pulse_next_frame < end_frame)
	{
		size_t offset = pulse_next_frame & (PULSE_RING_FRAMES - 1);
		size_t count = PULSE_RING_FRAMES - offset;

		if (count > writable)
			count = writable;
		if (count > end_frame - pulse_next_frame)
			count = end_frame - pulse_next_frame;

		if (pa_stream_write(pulse_stream, dma.buffer + offset * pulse_frame_bytes,
			count * pulse_frame_bytes, NULL, 0, PA_SEEK_RELATIVE) < 0)
		{
			Pulse_Error("write failed");
			return;
		}
		pulse_next_frame += count;
		pulse_queued = true;
		writable -= count;
	}
}

void S_PulseShutdown(void)
{
	if (pulse_stream)
	{
		pa_stream_disconnect(pulse_stream);
		pa_stream_unref(pulse_stream);
		pulse_stream = NULL;
	}
	if (pulse_context)
	{
		pa_context_disconnect(pulse_context);
		pa_context_unref(pulse_context);
		pulse_context = NULL;
	}
	if (pulse_loop)
	{
		pa_mainloop_free(pulse_loop);
		pulse_loop = NULL;
	}
	free(dma.buffer);
	memset(&dma, 0, sizeof(dma));
	pulse_failed = false;
	pulse_queued = false;
}

qboolean S_PulseInit(int speed, int bits, int channels)
{
	pa_sample_spec spec;
	pa_buffer_attr attr;

	spec.format = bits == 8 ? PA_SAMPLE_U8 : PA_SAMPLE_S16NE;
	spec.rate = speed ? speed : 44100;
	spec.channels = channels == 1 ? 1 : 2;
	if (!pa_sample_spec_valid(&spec))
	{
		Com_Printf("PulseAudio: invalid sample rate %d.\n", LOG_CLIENT, speed);
		return false;
	}

	pulse_loop = pa_mainloop_new();
	if (!pulse_loop)
		goto fail;
	pulse_context = pa_context_new(pa_mainloop_get_api(pulse_loop), "R1Q2");
	if (!pulse_context)
		goto fail;
	if (pa_context_connect(pulse_context, NULL, PA_CONTEXT_NOAUTOSPAWN, NULL) < 0 ||
		!Pulse_WaitReady())
		goto fail;

	pulse_stream = pa_stream_new(pulse_context, "Game audio", &spec, NULL);
	if (!pulse_stream)
		goto fail;
	attr.maxlength = (uint32_t)-1;
	attr.tlength = pa_usec_to_bytes(50000, &spec);
	attr.prebuf = pa_usec_to_bytes(20000, &spec);
	attr.minreq = (uint32_t)-1;
	attr.fragsize = (uint32_t)-1;
	if (pa_stream_connect_playback(pulse_stream, NULL, &attr, PA_STREAM_ADJUST_LATENCY,
		NULL, NULL) < 0 || !Pulse_WaitReady())
		goto fail;

	pulse_frame_bytes = pa_frame_size(&spec);
	dma.buffer = malloc(PULSE_RING_FRAMES * pulse_frame_bytes);
	if (!dma.buffer)
	{
		Com_Printf("PulseAudio: could not allocate DMA buffer.\n", LOG_CLIENT);
		pulse_failed = true;
		goto fail;
	}
	memset(dma.buffer, spec.format == PA_SAMPLE_U8 ? 0x80 : 0,
		PULSE_RING_FRAMES * pulse_frame_bytes);
	dma.channels = spec.channels;
	dma.samples = PULSE_RING_FRAMES * spec.channels;
	dma.submission_chunk = 1;
	dma.samplebits = pa_sample_size(&spec) * 8;
	dma.speed = spec.rate;
	dma.frame_time = Pulse_FrameTime;
	dma.submit_frames = Pulse_Submit;
	dma.clear_stream = Pulse_Clear;
	pulse_next_frame = 0;
	pulse_start = Pulse_Nanoseconds();
	Com_Printf("PulseAudio initialized.\n", LOG_CLIENT);
	return true;

fail:
	Pulse_Error("initialization failed");
	S_PulseShutdown();
	return false;
}
