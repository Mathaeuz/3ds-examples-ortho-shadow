#include <3ds.h>
#include <citro3d.h>
#include <tex3ds.h>
#include <string.h>
#include <stdio.h>
#include "texture_t3x.h"
#include "vshader_shbin.h"

#define CLEAR_COLOR 0x68B0D8FF

#define DISPLAY_TRANSFER_FLAGS                                                                     \
	(GX_TRANSFER_FLIP_VERT(0) | GX_TRANSFER_OUT_TILED(0) | GX_TRANSFER_RAW_COPY(0) |               \
	 GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGB8) | \
	 GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO))

typedef struct
{
	float position[3];
	float texcoord[2];
	float normal[3];
} vertex;

static const vertex vertex_list[] =
	{
		// First face (PZ)
		// First triangle
		{{-0.5f, -0.5f, +0.5f}, {0.0f, 0.0f}, {0.0f, 0.0f, +1.0f}},
		{{+0.5f, -0.5f, +0.5f}, {1.0f, 0.0f}, {0.0f, 0.0f, +1.0f}},
		{{+0.5f, +0.5f, +0.5f}, {1.0f, 1.0f}, {0.0f, 0.0f, +1.0f}},
		// Second triangle
		{{+0.5f, +0.5f, +0.5f}, {1.0f, 1.0f}, {0.0f, 0.0f, +1.0f}},
		{{-0.5f, +0.5f, +0.5f}, {0.0f, 1.0f}, {0.0f, 0.0f, +1.0f}},
		{{-0.5f, -0.5f, +0.5f}, {0.0f, 0.0f}, {0.0f, 0.0f, +1.0f}},

		// Second face (MZ)
		// First triangle
		{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
		{{-0.5f, +0.5f, -0.5f}, {1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
		{{+0.5f, +0.5f, -0.5f}, {1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}},
		// Second triangle
		{{+0.5f, +0.5f, -0.5f}, {1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}},
		{{+0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}},
		{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},

		// Third face (PX)
		// First triangle
		{{+0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}, {+1.0f, 0.0f, 0.0f}},
		{{+0.5f, +0.5f, -0.5f}, {1.0f, 0.0f}, {+1.0f, 0.0f, 0.0f}},
		{{+0.5f, +0.5f, +0.5f}, {1.0f, 1.0f}, {+1.0f, 0.0f, 0.0f}},
		// Second triangle
		{{+0.5f, +0.5f, +0.5f}, {1.0f, 1.0f}, {+1.0f, 0.0f, 0.0f}},
		{{+0.5f, -0.5f, +0.5f}, {0.0f, 1.0f}, {+1.0f, 0.0f, 0.0f}},
		{{+0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}, {+1.0f, 0.0f, 0.0f}},

		// Fourth face (MX)
		// First triangle
		{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}},
		{{-0.5f, -0.5f, +0.5f}, {1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}},
		{{-0.5f, +0.5f, +0.5f}, {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}},
		// Second triangle
		{{-0.5f, +0.5f, +0.5f}, {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}},
		{{-0.5f, +0.5f, -0.5f}, {0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}},
		{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}},

		// Fifth face (PY)
		// First triangle
		{{-0.5f, +0.5f, -0.5f}, {0.0f, 0.0f}, {0.0f, +1.0f, 0.0f}},
		{{-0.5f, +0.5f, +0.5f}, {1.0f, 0.0f}, {0.0f, +1.0f, 0.0f}},
		{{+0.5f, +0.5f, +0.5f}, {1.0f, 1.0f}, {0.0f, +1.0f, 0.0f}},
		// Second triangle
		{{+0.5f, +0.5f, +0.5f}, {1.0f, 1.0f}, {0.0f, +1.0f, 0.0f}},
		{{+0.5f, +0.5f, -0.5f}, {0.0f, 1.0f}, {0.0f, +1.0f, 0.0f}},
		{{-0.5f, +0.5f, -0.5f}, {0.0f, 0.0f}, {0.0f, +1.0f, 0.0f}},

		// Sixth face (MY)
		// First triangle
		{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
		{{+0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
		{{+0.5f, -0.5f, +0.5f}, {1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}},
		// Second triangle
		{{+0.5f, -0.5f, +0.5f}, {1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}},
		{{-0.5f, -0.5f, +0.5f}, {0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}},
		{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
};

#define vertex_list_count (sizeof(vertex_list) / sizeof(vertex_list[0]))
typedef struct
{
	DVLB_s *dvlb;
	shaderProgram_s program;
	int uLoc_projection, uLoc_modelView, uLoc_light;
} Shader;

static Shader loadShader(u32 *data, u32 size)
{
	Shader shader;
	shader.dvlb = DVLB_ParseFile(data, size);
	shaderProgramInit(&shader.program);
	shaderProgramSetVsh(&shader.program, &shader.dvlb->DVLE[0]);
	C3D_BindProgram(&shader.program);

	// Get the location of the uniforms
	shader.uLoc_projection = shaderInstanceGetUniformLocation(shader.program.vertexShader, "projection");
	shader.uLoc_modelView = shaderInstanceGetUniformLocation(shader.program.vertexShader, "modelView");
	shader.uLoc_light = shaderInstanceGetUniformLocation(shader.program.vertexShader, "light");

	return shader;
}

static Shader shader;
static C3D_Mtx projection, lightProjection;

static void *vbo_data;
static float angleY = 200.0;

// Helper function for loading a texture from memory
static bool loadTextureFromMem(C3D_Tex *tex, C3D_TexCube *cube, const void *data, size_t size)
{
	Tex3DS_Texture t3x = Tex3DS_TextureImport(data, size, tex, cube, false);
	if (!t3x)
		return false;

	// Delete the t3x object since we don't need it
	Tex3DS_TextureFree(t3x);
	return true;
}

static void sceneInit(void)
{
	consoleInit(GFX_BOTTOM, 0);
	printf("Press left or right to rotate the light source.");

	// Vertex shaders
	shader = loadShader((u32 *)vshader_shbin, vshader_shbin_size);
	C3D_BindProgram(&shader.program);

	// Configure attributes for use with the vertex shader
	C3D_AttrInfo *attrInfo = C3D_GetAttrInfo();
	AttrInfo_Init(attrInfo);
	AttrInfo_AddLoader(attrInfo, 0, GPU_FLOAT, 3); // v0=position
	AttrInfo_AddLoader(attrInfo, 1, GPU_FLOAT, 2); // v1=texcoord
	AttrInfo_AddLoader(attrInfo, 2, GPU_FLOAT, 3); // v2=normal

	// Create the VBO (vertex buffer object)
	vbo_data = linearAlloc(sizeof(vertex_list));
	memcpy(vbo_data, vertex_list, sizeof(vertex_list));

	// Configure buffers
	C3D_BufInfo *bufInfo = C3D_GetBufInfo();
	BufInfo_Init(bufInfo);
	BufInfo_Add(bufInfo, vbo_data, sizeof(vertex), 3, 0x210);

	C3D_DepthTest(true, GPU_GREATER, GPU_WRITE_ALL);
}

void updateLight()
{
	C3D_Mtx lightView;
	Mtx_Identity(&lightView);
	Mtx_Translate(&lightView, 0, 0, -10.0f, true);
	Mtx_RotateZ(&lightView, C3D_AngleFromDegrees(180), true);
	Mtx_RotateX(&lightView, C3D_AngleFromDegrees(-15), true);
	Mtx_Rotate(&lightView, FVec4_New(0, 1, 0, 0), C3D_AngleFromDegrees(angleY), true);

	// Compute the light projection matrix
	Mtx_Identity(&lightProjection);
	Mtx_PerspTilt(&lightProjection, C3D_AngleFromDegrees(60.0f), 1.0, 0.01f, 250.0f, false);
	Mtx_Multiply(&lightProjection, &lightProjection, &lightView);

	C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, shader.uLoc_light, &lightProjection);

	printf("\x1b[2;1H");
	C3D_FVec vec = FVec4_New(0, 0, 10, 1);
	vec = Mtx_MultiplyFVec4(&lightProjection, vec);
	printf("(%+06.2f,%+06.2f,%+06.2f,%+06.2f)\r\n", vec.x, vec.y, vec.z, vec.w);
}

static void sceneRender()
{
	// Calculate the modelView matrix for each geometry
	C3D_Mtx modelView;

	// Draw center cube
	Mtx_Identity(&modelView);
	Mtx_RotateY(&modelView, C3D_AngleFromDegrees(45), true);
	C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, shader.uLoc_modelView, &modelView);
	C3D_DrawArrays(GPU_TRIANGLES, 0, vertex_list_count);

	// Draw floor
	Mtx_Identity(&modelView);
	Mtx_RotateY(&modelView, C3D_AngleFromDegrees(45), true);
	Mtx_Translate(&modelView, 0, -1, 0, true);
	Mtx_Scale(&modelView, 20, 1, 20);
	C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, shader.uLoc_modelView, &modelView);
	C3D_DrawArrays(GPU_TRIANGLES, 0, vertex_list_count);
}

static void updateProjection(float iod)
{
	Mtx_Identity(&projection);
	Mtx_PerspStereoTilt(&projection, C3D_AngleFromDegrees(60.0f), C3D_AspectRatioTop, 0.01f, 250.0f, iod, 3.0f, false);
	Mtx_Translate(&projection, 0, 0, -4, true);
	Mtx_RotateX(&projection, C3D_AngleFromDegrees(45), true);

	C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, shader.uLoc_projection, &projection);
}

static void sceneExit(void)
{
	// Free the VBO
	linearFree(vbo_data);

	// Free the shader program
	shaderProgramFree(&shader.program);
	DVLB_Free(shader.dvlb);
}

int main()
{
	// Initialize graphics
	gfxInitDefault();
	gfxSet3D(true); // Enable stereoscopic 3D
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE * 2);

	// Load regular texture
	C3D_Tex texture;
	if (!loadTextureFromMem(&texture, NULL, texture_t3x, texture_t3x_size))
		svcBreak(USERBREAK_PANIC);
	C3D_TexSetFilter(&texture, GPU_LINEAR, GPU_NEAREST);
	texture.border = 0;
	C3D_TexSetWrap(&texture, GPU_CLAMP_TO_BORDER, GPU_CLAMP_TO_BORDER);

	// Init 128x128 shadow texture (depth map)
	C3D_Tex shadowTex;
	C3D_TexInitShadow(&shadowTex, 256, 256);
	shadowTex.border = 0xffffffff;
	C3D_TexSetFilter(&shadowTex, GPU_LINEAR, GPU_LINEAR);
	C3D_TexSetWrap(&shadowTex, GPU_CLAMP_TO_BORDER, GPU_CLAMP_TO_BORDER);

	// Initialize the render targets
	C3D_RenderTarget *targetShadow = C3D_RenderTargetCreateFromTex(&shadowTex, GPU_TEXFACE_2D, 0, -1);
	C3D_RenderTarget *targetLeft = C3D_RenderTargetCreate(240, 400, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8);
	C3D_RenderTarget *targetRight = C3D_RenderTargetCreate(240, 400, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8);
	C3D_RenderTargetSetOutput(targetLeft, GFX_TOP, GFX_LEFT, DISPLAY_TRANSFER_FLAGS);
	C3D_RenderTargetSetOutput(targetRight, GFX_TOP, GFX_RIGHT, DISPLAY_TRANSFER_FLAGS);

	// Bind textures
	// C3D_TexBind(0, &shadowTex); // Depth map only works as shadow on unit0
	C3D_TexBind(0, &texture);

	// Initialize the scene
	sceneInit();

	// Main loop
	while (aptMainLoop())
	{
		hidScanInput();

		// Respond to user input
		u32 kDown = hidKeysDown();
		u32 kHeld = hidKeysHeld();
		if (kDown & KEY_START)
			break; // break in order to return to hbmenu

		// rotate light
		if (kHeld & KEY_RIGHT)
			angleY += 1;
		if (kHeld & KEY_LEFT)
			angleY -= 1;

		updateLight();

		float slider = osGet3DSliderState();
		float iod = slider / 3;
		C3D_TexEnv *env;

		// Render the scene
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		{
			// Shadow Pass
			// Shadow casting Geometry is rendered into a depth map target

			// Set fragment mode to shadow
			// Depth output can be sourced from GPU_FRAGMENT_PRIMARY_COLOR
			// Set depth capture scale and bias
			// Frontface culling for shadows
			// Prepare to draw on the target created from texShadow
			C3D_FragOpMode(GPU_FRAGOPMODE_SHADOW);

			env = C3D_GetTexEnv(0);
			C3D_TexEnvInit(env);
			C3D_TexEnvSrc(env, C3D_Both, GPU_FRAGMENT_PRIMARY_COLOR, GPU_PRIMARY_COLOR, GPU_PRIMARY_COLOR);
			C3D_TexEnvFunc(env, C3D_Both, GPU_REPLACE);

			C3D_FragOpShadow(0.0, 1.0);
			// Adjust depth mapping
			C3D_DepthMap(false, -1.0, 0.0);
			C3D_CullFace(GPU_CULL_FRONT_CCW);

			C3D_RenderTargetClear(targetShadow, C3D_CLEAR_ALL, 0xFFFFFFFF, 0);
			C3D_FrameDrawOn(targetShadow);
			C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, shader.uLoc_projection, &lightProjection);
			sceneRender();
			C3D_FrameSplit(0);

			// Color Pass
			// Depthmap at TEXTURE0 is compared by against the depth provided by the shader via texcoord0w
			// and applied as shadow

			// Set fragment mode to regular
			// GPU_TEXTURE0 * GPU_TEXTURE1 as source
			// Reset depth map config
			// Backface culling
			// Perspective shadows, and no bias (0.0). *Fine tune bias acording to use case.

			C3D_FragOpMode(GPU_FRAGOPMODE_GL);

			env = C3D_GetTexEnv(0);
			C3D_TexEnvInit(env);
			C3D_TexEnvSrc(env, C3D_RGB, GPU_TEXTURE0, GPU_PRIMARY_COLOR, GPU_PRIMARY_COLOR);
			C3D_TexEnvFunc(env, C3D_RGB, GPU_REPLACE);

			C3D_DepthMap(true, -1.0f, 0.0f);
			C3D_CullFace(GPU_CULL_BACK_CCW);
			C3D_TexShadowParams(true, 0.0);

			// Left eye/default target
			C3D_RenderTargetClear(targetLeft, C3D_CLEAR_ALL, CLEAR_COLOR, 0);
			C3D_FrameDrawOn(targetLeft);
			updateProjection(-iod);
			sceneRender();

			// Right eye
			if (iod > 0.0f)
			{
				C3D_RenderTargetClear(targetRight, C3D_CLEAR_ALL, CLEAR_COLOR, 0);
				C3D_FrameDrawOn(targetRight);
				updateProjection(iod);
				sceneRender();
			}
		}
		C3D_FrameEnd(0);
	}

	// Deinitialize the scene
	sceneExit();
	C3D_RenderTargetDelete(targetShadow);
	C3D_RenderTargetDelete(targetLeft);
	C3D_RenderTargetDelete(targetRight);
	C3D_TexDelete(&shadowTex);
	C3D_TexDelete(&texture);

	// Deinitialize graphics
	C3D_Fini();
	gfxExit();
	return 0;
}
