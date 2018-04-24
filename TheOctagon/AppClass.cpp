#include "AppClass.h"
using namespace Simplex;
void Application::InitVariables(void)
{
	// Sets Camera at the Y Perspective
	m_pCameraMngr->SetCameraMode(CAM_ORTHO_Y);
	m_pCameraMngr->SetPosition(vector3(0.0f, 30.0f, 0.0f));

	m_pLightMngr->SetPosition(vector3(0.0f, 3.0f, 13.0f), 1); //set the position of first light (0 is reserved for ambient light)

#ifdef DEBUG
	uint uInstances = 9;
#else
	uint uInstances = 16;
#endif
	float radiusObj = 20.0f;
	float radiusCubes = 50.0f;
	int numCubes = 270; // Number of Cubes for Barrier
	int nSquare = static_cast<int>(std::sqrt(uInstances));	
	m_uObjects = nSquare * nSquare; // Number of Object in the Barrier
	uint uIndex = -1;

	// Get all objects in a 2D Circle.
	for (int i = 0; i < m_uObjects; i++)
	{
		double angle = 2 * PI * i / m_uObjects;	//Angle between vertices (based on subdivisions)
		double x = cosf(angle) * radiusObj;	//Get xPosition
		double z = sinf(angle) * radiusObj;	//Get zPosition

		uIndex++;
		m_pEntityMngr->AddEntity("Sorted\\IronMan.obj");
		vector3 v3Position = vector3(x, 0.0f, z);
		m_pEntityMngr->GetEntity()->tag = "Cube";
		m_pEntityMngr->GetEntity()->position = v3Position;
		m_pEntityMngr->GetEntity()->GetRigidBody()->SetColorNotColliding(vector3((float)rand() / RAND_MAX));		
		matrix4 m4Position = glm::translate(m_pEntityMngr->GetEntity()->position);

		m_pEntityMngr->SetModelMatrix(m4Position);
	}

	// Get all of the points on the 2D circle.
	for (int i = 0; i < numCubes; i++) 
	{	
		double angle = 2 * PI * i / numCubes;	//Angle between vertices (based on subdivisions)
		double x = cosf(angle) * radiusCubes;	//Get xPosition
		double z = sinf(angle) * radiusCubes;	//Get zPosition

		m_pEntityMngr->AddEntity("Minecraft\\Cube.obj");
		vector3 temp_v3Position = vector3(x, 0.0f, z);
		m_pEntityMngr->GetEntity()->tag = "Wall";
		m_pEntityMngr->GetEntity()->position = temp_v3Position;
		matrix4 temp_m4Position = glm::translate(m_pEntityMngr->GetEntity()->position);

		m_pEntityMngr->SetModelMatrix(temp_m4Position);
	}

	m_uOctantLevels = 1;
	m_pRoot = new MyOctant(m_uOctantLevels, 5);
	m_pEntityMngr->Update();
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the ArcBall active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();

	//Update Entity Manager
	m_pEntityMngr->Update();

	//Add objects to render list
	m_pEntityMngr->AddEntityToRenderList(-1, true);
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	// Display octree
	if (visual)
	{
		if (m_uOctantID == -1)
			m_pRoot->Display();
		else
			m_pRoot->Display(m_uOctantID);
	}

	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();

	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();

	//draw gui,
	DrawGUI();

	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	//release GUI
	ShutdownGUI();
}