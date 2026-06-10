#pragma once

typedef enum {
	ErrorClear,
	ErrorNull,
	ErrorOutOfBounds,
	ErrorNoWinForScene,
	ErrorSceneIndexNotFound,
	ErrorFileAccessFailed,
	ErrorInadequateBufferSize,
	ErrorFailedAlloc,

	GCErrorTypeCount,
} GCError;
