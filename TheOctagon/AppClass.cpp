#include "AppClass.h"
using namespace Simplex;
void Application::InitVariables(void)
{
	//Sets Camera at the Y Perspective
	m_pCameraMngr->SetCameraMode(CAM_ORTHO_Y);

	m_pLightMngr->SetPosition(vector3(0.0f, 3.0f, 13.0f), 1); //set the position of first light (0 is reserved for ambient light)

#ifdef DEBUG
	uint uInstances = 9;
#else
	uint uInstances = 16;
#endif
	float spaceCounter = -8.0f; // Manipulate the position of each object
	int nSquare = static_cast<int>(std::sqrt(uInstances));
	m_uObjects = nSquare * nSquare;
	uint uIndex = -1;
	for (int i = 0; i < nSquare; i++)
	{
		for (int j = 0; j < nSquare; j++)
		{
			uIndex++;
			m_pEntityMngr->AddEntity("Sorted\\IronMan.obj");
			vector3 v3Position = vector3(spaceCounter, 0.0f, 0.0f);
			m_pEntityMngr->GetEntity()->position = v3Position;
			m_pEntityMngr->GetEntity()->GetRigidBody()->SetColorNotColliding(vector3((float)rand() / RAND_MAX));
			m_pEntityMngr->GetEntity()->tag = "Cube";
			matrix4 m4Position = glm::translate(m_pEntityMngr->GetEntity()->position);
			m_pEntityMngr->SetModelMatrix(m4Position);
			spaceCounter += 2.0f;
		}
	}

	// Cube Barrier
	int numCubes = 300;
	float radius = 30.0f;

	for (int i = 0; i < numCubes; i++) {	//Get all of the points on the 2D circle.
		double angle = 2 * PI * i / numCubes;	//Angle between vertices (based on subdivisions)
		double x = cosf(angle) * radius;	//Get xPosition
		double z = sinf(angle) * radius;	//Get zPosition

		m_pEntityMngr->AddEntity("Minecraft\\Cube.obj");
		vector3 tempPosition = vector3(x, 0.0f, z);
		m_pEntityMngr->GetEntity()->tag = "Wall";
		m_pEntityMngr->GetEntity()->position = tempPosition;
		matrix4 tempm4Position = glm::translate(m_pEntityMngr->GetEntity()->position);
		m_pEntityMngr->SetModelMatrix(tempm4Position);
	}

	////Left wall
	//m_pEntityMngr->AddEntity("Minecraft\\Cube.obj");
	//m_pEntityMngr->GetEntity()->tag = "Wall";
	//vector3 v3Position1 = vector3(-10, 0, 0);
	//m_pEntityMngr->GetEntity()->position = v3Position1;
	//matrix4 m4Position1 = glm::translate(m_pEntityMngr->GetEntity()->position);
	//m_pEntityMngr->SetModelMatrix(m4Position1 * glm::scale(vector3(3.0f)));

	////Right wall
	//m_pEntityMngr->AddEntity("Minecraft\\Cube.obj");
	//m_pEntityMngr->GetEntity()->tag = "Wall";
	//vector3 v3Position2 = vector3(10, 0, 0);
	//m_pEntityMngr->GetEntity()->position = v3Position2;
	//matrix4 m4Position2 = glm::translate(m_pEntityMngr->GetEntity()->position);
	//m_pEntityMngr->SetModelMatrix(m4Position2 * glm::scale(vector3(3.0f)));

	////Top Wall
	//m_pEntityMngr->AddEntity("Minecraft\\Cube.obj");
	//m_pEntityMngr->GetEntity()->tag = "Wall";
	//vector3 v3Position3 = vector3(0, 0, -10);
	//m_pEntityMngr->GetEntity()->position = v3Position3;
	//matrix4 m4Position3 = glm::translate(m_pEntityMngr->GetEntity()->position);
	//m_pEntityMngr->SetModelMatrix(m4Position3 * glm::scale(vector3(3.0f)));

	////Bottom Wall
	//m_pEntityMngr->AddEntity("Minecraft\\Cube.obj");
	//m_pEntityMngr->GetEntity()->tag = "Wall";
	//vector3 v3Position4 = vector3(0, 0, 10);
	//m_pEntityMngr->GetEntity()->position = v3Position4;
	//matrix4 m4Position4 = glm::translate(m_pEntityMngr->GetEntity()->position);
	//m_pEntityMngr->SetModelMatrix(m4Position4 * glm::scale(vector3(300.0f)));

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

	//display octree
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