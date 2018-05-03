
#include <scene/SceneProp.h>

#include <video/gl/GLShader.h>
#include <video/gl/GLDriver.h>

#if defined(OS_WINDOWS)
#include <video/d3d11/D3DXShader.h>
#include <video/d3d11/D3DXDriver.h>
#endif

#if defined(OS_ANDROID)
static u_long myNextRandom = 1;
#ifndef atof
double atof(const char *nptr)
{
	return (strtod(nptr, NULL));
}
#endif 
#ifdef rand
#undef rand
int rand(void)
{
	return (int)((myNextRandom = (1103515245 * myNextRandom) + 12345) % ((u_long)RAND_MAX + 1));
}
#endif

#ifdef srand
#undef rand
void srand(u_int seed)
{
	myNextRandom = seed;
}
#endif
#endif 

namespace t1000 {
	extern Device*            T8Device;
	extern DeviceContext*     T8DeviceContext;
}

void	SceneProps::AddLight(XVECTOR3 Pos, XVECTOR3 Color,float radius, bool enabled){
	Light l;
	l.Position=Pos;
	l.Color=Color;
	l.Enabled=(int)enabled;
  l.radius = radius;
	Lights.push_back(l);
}

void	SceneProps::RemoveLight(unsigned int index){
	if(index < 0 || index >= Lights.size())
		return;

	Lights.erase(Lights.begin() + index);
}

void	SceneProps::SetLightPos(unsigned int index, XVECTOR3 pos){
	if (index < 0 || index >= Lights.size())
		return;

	Lights[index].Position = pos;
}

void	SceneProps::AddCamera(Camera* cam){
	pCameras.push_back(cam);
}

void	SceneProps::RemoveCamera(unsigned int index){
	if (index < 0 || index >= pCameras.size())
		return;

	pCameras.erase(pCameras.begin() + index);
}

void	SceneProps::AddLightCamera(Camera* cam) {
	pLightCameras.push_back(cam);
}

void	SceneProps::RemoveLightCamera(unsigned int index) {
	if (index < 0 || index >= pLightCameras.size())
		return;

	pLightCameras.erase(pLightCameras.begin() + index);
}

template<typename T>
T RandRange(T m, T M) {
	T r = static_cast <T> (rand()) / static_cast <T> (RAND_MAX);
	return m + r * (M - m);
}


float lerp(float a, float b, float f)
{
	return a + f * (b - a);
}

void SSAOFilter::InitTexture() {
	unsigned char *pChar = &Noise[0];
	for (int i = 0; i < SSAO_NOISE_SIZE*SSAO_NOISE_SIZE; i++) {
			*pChar = (unsigned char)RandRange(0.0f,255.0f); pChar++;
			*pChar = (unsigned char)RandRange(0.0f,255.0f); pChar++;
			*pChar = 0; pChar++;
			*pChar = 255; pChar++;
		
	}
	string dummy;
	NoiseTex = t1000::T8Device->CreateTextureFromMemory(Noise, SSAO_NOISE_SIZE, SSAO_NOISE_SIZE, 4, dummy);
	NoiseTex->params |= t1000::T_TEXT_BASIC_PARAMS::TILED;
	NoiseTex->SetTextureParams();
}

void SSAOFilter::Update() {
	vSSAOKernel.clear();
	for (int i = 0; i < KernelSize; i++) {
		XVECTOR3 vec = XVECTOR3(
			RandRange(-1.0f, 1.0f), 
			RandRange(-1.0f, 1.0f),
			RandRange( 0.0f, 1.0f)
		);
		vec.Normalize();
		vec.x *= RandRange(0.0f, 1.0f);
		vec.y *= RandRange(0.0f, 1.0f);
		vec.z *= RandRange(0.0f, 1.0f);

		float scale = float(i) / float(KernelSize);
		scale = lerp(0.1f, 1.0f, scale * scale);
		vec.x *= scale;
		vec.y *= scale;
		vec.z *= scale;
		vSSAOKernel.push_back(vec);
	}
}

void	GaussFilter::Update(){	
	if ((kernelSize - 1) % 2 != 0)
		kernelSize--;

	std::vector<sample_> allSamples = UpdateKernel(sigma, (float)kernelSize, 1000.0f);
	XVECTOR3 KernelSize = XVECTOR3((float)allSamples.size(), radius, 0.0f, 0.0f);

	vGaussKernel.clear();
	vGaussKernel.push_back(KernelSize);
	for (unsigned int i = 0; i < allSamples.size(); i++) {
		vGaussKernel.push_back(XVECTOR3(allSamples[i].weight, 0.0f, 0.0f, 0.0f));
	}
}

void SceneProps::AddGaussKernel(GaussFilter* pGF){
	pGaussKernels.push_back(pGF);
}
