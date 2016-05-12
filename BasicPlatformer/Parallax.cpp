#include "Parallax.hpp"
#include <cmath>

ParallaxPiece::ParallaxPiece(RenderingSystem::Texture* t, float followRatio, bool autoWalk, vec2 position)
{
	this->t = t;

	// Calculate amount of pieces needed
	amount = (RenderingSystem::GetResolution().x / t->GetSize().x) + 1u;
	Xpositions = new float[amount];
	Xpositions[0] = RenderingSystem::GetViewportPosition().x;
	Yposition = position.y;
	for(dword i = 1; i < amount; i++)
		Xpositions[i] = Xpositions[0]
			+ (static_cast<float>(t->GetSize().x) * static_cast<float>(i));
	this->followRatio = followRatio;
	this->autoWalk = autoWalk;
}

ParallaxPiece::~ParallaxPiece()
{
	delete[] Xpositions;
	RenderingSystem::TexturePool::DisposeTexture(t);
}



void Parallax::Update()
{
	float vwpXpos = RenderingSystem::GetViewportPosition().x;
	for (auto piecePtr : pieces)
	{
		if(!piecePtr->autoWalk)
			piecePtr->Xpositions[0] = vwpXpos * piecePtr->followRatio;
		else piecePtr->Xpositions[0] -= 1.0f - piecePtr->followRatio;

		// Exceeding
		if((piecePtr->Xpositions[0] <
			vwpXpos - static_cast<float>(piecePtr->t->GetSize().x))
			|| (piecePtr->Xpositions[0] > vwpXpos))
		{
			// Calculate number of times for the value to be valid
			float diff = vwpXpos - static_cast<float>(piecePtr->t->GetSize().x);
			diff -= piecePtr->Xpositions[0];
			int times = fabs(diff) / static_cast<float>(piecePtr->t->GetSize().x);
			times++;
			times *= diff / fabs(diff);
			piecePtr->Xpositions[0] += times * static_cast<float>(piecePtr->t->GetSize().x);
		}

		for (dword i = 1; i < piecePtr->amount; i++)
		{
			piecePtr->Xpositions[i] = piecePtr->Xpositions[0]
				+ (static_cast<float>(piecePtr->t->GetSize().x)
					* static_cast<float>(i));
		}
	}
}

void Parallax::UnloadContent()
{
	for (auto piecePtr : pieces)
		delete piecePtr;
	pieces.clear();
}

void Parallax::Draw()
{
	glPushMatrix();
	for (auto piecePtr : pieces)
		for (dword i = 0; i < piecePtr->amount; i++)
		{
			glPushMatrix();
				glTranslatef(piecePtr->Xpositions[i], piecePtr->Yposition + RenderingSystem::GetViewportPosition().y, 0.0f);
				glTranslatef(0.0f, yPos, 0.0f);
				piecePtr->t->Draw(vec2::Zero(),	Color4::MaskToColor4(WHITE));
			glPopMatrix();
		}
	glPopMatrix();
}

void Parallax::AppendPiece(ParallaxPiece* p)
{
	pieces.push_back(p);
}

float Parallax::GetYPosition() const
{
	return yPos;
}

void Parallax::SetYPosition(float ypos)
{
	yPos = ypos;
}
