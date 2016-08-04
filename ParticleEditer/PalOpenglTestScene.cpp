
#include "PalOpenglTestScene.h"
#include <gl\GL.h>
#include <gl\GLU.h>
#include "..\state\DexGameEngine.h"
#include "..\DexBase\IDexDevice.h"

PalOpenGLTestScene::PalOpenGLTestScene()
{

}

PalOpenGLTestScene::~PalOpenGLTestScene()
{

}
static int z = 10;
bool PalOpenGLTestScene::ApplyRes()
{
	DexVector3 eye = DexVector3(0, 0, z);
	DexVector3 lookAt = DexVector3(0.0f, 0.0f, 0.0f);
	DexVector3 up = DexVector3(0.0f, 1.0f, 0.0f);
	DexGameEngine::getEngine()->LookAtLH(&eye, &lookAt, &up);
	return true;
}

void PalOpenGLTestScene::ReleaseRes()
{

}

bool PalOpenGLTestScene::Update(int delta)
{
	return true;
}

void PalOpenGLTestScene::BeginRender()
{

}

void PalOpenGLTestScene::Render()
{
	DexMatrix4x4 worldMatrix;
	worldMatrix.Translate(5, 0, 0);
	DexGameEngine::getEngine()->GetDDevice()->SetTransform(DEXTRANSFORMSTATETYPE_WORLD, &worldMatrix);
	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
	glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 0.0f);
	glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(1.0f, -1.0f, 0.0f);
	glEnd();

	worldMatrix.Identity();
	worldMatrix.Translate(-5.0f, 0.0f, 0.0f);
	DexGameEngine::getEngine()->GetDDevice()->SetTransform(DEXTRANSFORMSTATETYPE_WORLD, &worldMatrix);
	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
	glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 0.0f);
	glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(1.0f, -1.0f, 0.0f);
	glEnd();
}

void PalOpenGLTestScene::EndRender()
{

}

void PalOpenGLTestScene::MouseLDown(int xPos, int yPos)
{
	z++;
	DexVector3 eye = DexVector3(0, 0, z);
	DexVector3 lookAt = DexVector3(0.0f, 0.0f, 0.0f);
	DexVector3 up = DexVector3(0.0f, 1.0f, 0.0f);
	DexGameEngine::getEngine()->LookAtLH(&eye, &lookAt, &up);
}

