#include "Fighter.h"

void Fighter::Serialize(IArchive& archive)
{
	archive.Serialize(name, "name");
	archive.Serialize(spriteSheet, "spriteSheet");
	archive.Serialize(moves, "moves");
	archive.Serialize(initialFrame, "initialFrame");
	archive.Serialize(spriteFlipped, "spriteFlipped");
	archive.Serialize(scale, "scale");

	if (archive.IsOutput())
	{

	}

	if (archive.IsInput()) 
	{

	}
}