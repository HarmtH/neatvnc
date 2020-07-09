#pragma once

#include <unistd.h>
#include <stdint.h>
#include <zlib.h>
#include <pthread.h>

struct tight_tile;
struct pixman_region16;
struct aml_work;

enum tight_quality {
	TIGHT_QUALITY_UNSPEC = 0,
	TIGHT_QUALITY_LOSSLESS,
	TIGHT_QUALITY_LOW,
	TIGHT_QUALITY_HIGH,
};

struct tight_encoder_v2 {
	uint32_t width;
	uint32_t height;
	uint32_t grid_width;
	uint32_t grid_height;
	enum tight_quality quality;

	struct tight_tile* grid;

	z_stream zs[4];
	struct aml_work* zs_worker[4];

	const struct rfb_pixel_format* dfmt;
	const struct rfb_pixel_format* sfmt;
	const struct nvnc_fb* fb;

	uint32_t n_rects;
	uint32_t n_jobs;

	struct vec* dst;

	pthread_mutex_t wait_mutex;
	pthread_cond_t wait_cond;
};

int tight_encoder_v2_init(struct tight_encoder_v2* self, uint32_t width,
		uint32_t height);
void tight_encoder_v2_destroy(struct tight_encoder_v2* self);

int tight_encode_frame_v2(struct tight_encoder_v2* self, struct vec* dst,
		const struct rfb_pixel_format* dfmt,
		const struct nvnc_fb* src,
		const struct rfb_pixel_format* sfmt,
		struct pixman_region16* damage,
		enum tight_quality quality);
