
#include "../Include/TextureContainer.h"
#include "../../ThirdParty/soil/SOIL.h"

int TextureContainer::LoadTexture(string Path,bool mipmaps,bool dxt)
{

	for (int i = 0; i < NumTexturesLoaded ; i++)
	{
		if( Container[i].Path.compare(Path) == 0 )
		{
			return i;
		}
	}

	unsigned int params = 0,id;

	if(mipmaps)
		params |=  SOIL_FLAG_MIPMAPS;

	if(dxt)
		params |= SOIL_FLAG_COMPRESS_TO_DXT;

	id = SOIL_load_OGL_texture(Path.c_str(),SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,params);

	
	glBindTexture(GL_TEXTURE_2D,id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

		if(mipmaps)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		else
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	FTTexture tmp;
	tmp.Path = Path;
	tmp.Id = id;

	LastIndex++;
	NumTexturesLoaded++;
	Container.push_back(tmp);
	cout << "loaded texture: " << Path << " con index " << LastIndex <<  endl;

	return LastIndex;

}

FTTexture	TextureContainer::GetTexture(int id)
{
	if( (id >= 0) && (id <= LastIndex))
	{
		return Container[id];
	}else
	{
		return Container[0];
	}
}

void TextureContainer::ReleaseTextures()
{
	for (int i = 0; i < Container.size() ; i++)
	{
		glDeleteTextures(1,&Container[i].Id);
	}
	
	Container.clear();
}

TextureContainer* TextureContainer::Instance()
{
	static TextureContainer instance;
	return &instance;
}

