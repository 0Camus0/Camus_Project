/*=============================================================================
TextureContainer.h    : Texture Container.
Revision history:
* Created by Camus
=============================================================================*/

#ifndef TEXTURECONTAINER_H
#define TEXTURECONTAINER_H

#include "GlobalHeader.h"

using namespace std;

typedef unsigned char TData;
#include <string>
#include <vector>
using namespace std;

struct FTTexture{
	FTTexture(){ }
	int		Height;
	int		Width;
	int		Channels;
	unsigned int	Id;
	string	Path;
};

class TextureContainer
{
public:
	int					LoadTexture(string Path,bool mipmaps,bool dxt);
	FTTexture			GetTexture(int id);
	void				ReleaseTextures();

	static TextureContainer*	Instance();

private:

	TextureContainer(){ LastIndex = -1; NumTexturesLoaded = 0; }

	int					LastIndex;
	int					NumTexturesLoaded;
	vector<FTTexture> Container;

};

#define TXCONTAINER TextureContainer::Instance()
#define LOADTEXTURE(x) TextureContainer::Instance()->LoadTexture(x)
#define GETTEXTURE(x) TextureContainer::Instance()->GetTexture(x)

#endif
