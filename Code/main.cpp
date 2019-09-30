#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include "Material.h"
#include "Mesh.h"
#include "Transform.h"
#include "Texture.h"
#include "Camera.h"
#include "FrameBuffer.h"
#include "Screen.h"
#include "CubeMap.h"
#include "PointLight.h"
#include "SpotLight.h"

bool ShouldQuit = false;
bool KeyState[GLFW_KEY_LAST] = { false};
bool CameraMovementActive = false;
bool ActivateGauissBluri = false;

double MouseX = 0.0;
double MouseY = 0.0;

Camera TheCamera;
Material DefaultMaterial;
Material SkyboxMaterial;
Mesh QuadMesh;
Mesh CubeMesh;
Mesh TriangleMesh;
Texture CherylTexture;
CubeMap SkyboxTexture;
Material LightSourceMaterial;
PointLight MainLight;
PointLight SecondLight;
SpotLight FlashLight;
Material GaussisMaterial;


const float CubeVertexData[] = {
	//FRONTFACE
	-1.f, -1.f, +1.f,	0.f, 0.f, 1.f,	0.f, 0.f,//0
	+1.f, -1.f, +1.f,	0.f, 0.f, 1.f,	1.f, 0.f,//1
	+1.f, +1.f, +1.f,	0.f, 0.f, 1.f,	1.f, 1.f,//2
	-1.f, +1.f, +1.f,	0.f, 0.f, 1.f,	0.f, 1.f,//3
	//BACKFACE
	-1.f, -1.f, -1.f,	0.f, 0.f, -1.f,	0.f, 0.f,//4
	+1.f, -1.f, -1.f,	0.f, 0.f, -1.f,	1.f, 0.f,//5
	+1.f, +1.f, -1.f,	0.f, 0.f, -1.f,	1.f, 1.f,//6
	-1.f, +1.f, -1.f,	0.f, 0.f, -1.f,	0.f, 1.f,//7
	//RIGHTFACE
	+1.f, -1.f, +1.f,	+1.f, 0.f, 0.f,	0.f, 0.f,//8
	+1.f, -1.f, -1.f,	+1.f, 0.f, 0.f,	1.f, 0.f,//9
	+1.f, +1.f, -1.f,	+1.f, 0.f, 0.f,	1.f, 1.f,//10
	+1.f, +1.f, +1.f,	+1.f, 0.f, 0.f,	0.f, 1.f,//11
	//LEFTFACE
	-1.f, -1.f, +1.f,	-1.f, 0.f, 0.f,	0.f, 0.f,//12
	-1.f, -1.f, -1.f,	-1.f, 0.f, 0.f,	1.f, 0.f,//13
	-1.f, +1.f, -1.f,	-1.f, 0.f, 0.f,	1.f, 1.f,//14
	-1.f, +1.f, +1.f,	-1.f, 0.f, 0.f,	0.f, 1.f,//15
	//TOPFACE
	-1.f, +1.f, +1.f,	0.f, 1.f, 0.f,	0.f, 0.f,//16
	+1.f, +1.f, +1.f,	0.f, 1.f, 0.f,	1.f, 0.f,//17
	+1.f, +1.f, -1.f,	0.f, 1.f, 0.f,	1.f, 1.f,//18
	-1.f, +1.f, -1.f,	0.f, 1.f, 0.f,	0.f, 1.f,//19
	//BOTTOMFACE
	-1.f, -1.f, +1.f,	0.f, -1.f, 0.f,	0.f, 0.f,//20
	+1.f, -1.f, +1.f,	0.f, -1.f, 0.f,	1.f, 0.f,//21
	+1.f, -1.f, -1.f,	0.f, -1.f, 0.f,	1.f, 1.f,//22
	-1.f, -1.f, -1.f,	0.f, -1.f, 0.f,	0.f, 1.f,//23
};

const unsigned int CubeIndexData[] = {
	//Frontface
	0, 1, 2,	0, 2, 3,
	//Backface
	4, 5, 6,	4, 6, 7,
	//Rightface
	8, 9, 10,	8, 10, 11,
	//Leftface
	12, 13, 14, 12, 14, 15,
	//Topface
	16, 17, 18, 16, 18, 19,
	//Bottomface
	20, 21, 22, 20, 22, 23,


};

const char* SkyboxImages[] = {
	"Res/Skybox/blood_ft.tga", //Positive X Front
	"Res/Skybox/blood_bk.tga", //Negative X Back
	"Res/Skybox/blood_up.tga", //Positive Y Up
	"Res/Skybox/blood_dn.tga", //Negative Y Down
	"Res/Skybox/blood_rt.tga", //Positive Z Right
	"Res/Skybox/blood_lf.tga"  //Negative Z Left
};

bool IsKeyPressed(int Key)
{
	return KeyState[Key];
}

//int QuitSequence[] = { GLFW_KEY_Q, GLFW_KEY_U, GLFW_KEY_I, GLFW_KEY_T };
//int QuitSequenceIndex = 0;

void OnKeyEvent(GLFWwindow* Window, int Key, int Scancode, int Action, int Mods)
{
	if (Action == GLFW_REPEAT)
	{
		return;
	}

	KeyState[Key] = (Action == GLFW_PRESS);

	if (Action == GLFW_PRESS)
	{
		
	}


	if (Action == GLFW_PRESS)
	{
		printf("OnKeyEvent( %d)\n", Key);

		if (Key == GLFW_KEY_ESCAPE)
		{
			ShouldQuit = true;
		}

		/*if (Key == QuitSequence[QuitSequenceIndex])
		{
			QuitSequenceIndex++;
			if (QuitSequenceIndex >= 4)
			{
				ShouldQuit = true;
			}
		}
		else
		{
			QuitSequenceIndex = 0;
		}
*/
	}

}

void OnMouseButton(GLFWwindow* Window, int Button, int Action, int Mods)
{

	if (Button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		CameraMovementActive = (Action == GLFW_PRESS);
		glfwSetInputMode(Window, GLFW_CURSOR, CameraMovementActive ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
	}

}

void OnCursorMove(GLFWwindow* Window, double X, double Y)
{
	MouseX = X;
	MouseY = Y;

}

void RenderQuad(const Transform& Transform)
{
	QuadMesh.Bind();
	DefaultMaterial.Set("u_World", Transform.GetMatrix());
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

}

void RenderCube(const Transform& Transform)
{
	CubeMesh.Bind();
	DefaultMaterial.Set("u_World", Transform.GetMatrix());
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	
}

void RenderTraingle(const Transform& Transform)
{
	TriangleMesh.Bind();
	DefaultMaterial.Set("u_World", Transform.GetMatrix());
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void RenderFullScreenQuad()
{
	glBindVertexArray(0);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);


}

void RenderSkybox()
{

	SkyboxTexture.Bind();
	CubeMesh.Bind();
	SkyboxMaterial.Use();
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

}

void RenderLightSource(const PointLight& Light)
{
	LightSourceMaterial.Use();
	LightSourceMaterial.Set("u_World", Transform(Light.Position, quat_identity, glm::vec3(0.2f)).GetMatrix());
	LightSourceMaterial.Set("u_Color", Light.Color);
	CubeMesh.Bind();
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

}


void RenderScene(bool RenderPlayer)
{
	CherylTexture.Bind();

	float Time = glfwGetTime();

	

	//RenderFloor
	RenderCube(Transform(glm::vec3(5.f, -1.f, 0.f), quat_identity, glm::vec3(50.f, 0.4f, 50.f)));
	//Chile And Baby and BadAss Cubes
	{

		Transform BaseTransform;
		BaseTransform.Position = glm::vec3(5.f, 5.f, glm::sin(Time) * 5.f);
		BaseTransform.Rotation = glm::angleAxis(Time, glm::vec3(0.f, 1.f, 0.f));

		Transform ChildTransform;
		ChildTransform.Position = glm::vec3(2.f + glm::sin(Time), 0.f, 0.f);
		ChildTransform.Scale = glm::vec3(0.5f);
		ChildTransform.Rotation = glm::angleAxis(Time, glm::vec3(1.f, 0.f, 0.f));

		Transform BabyTransform;
		BabyTransform.Position = glm::vec3(0.f, 2.5f, 0.f);
		BabyTransform.Scale = glm::vec3(0.5f);

		Transform BadAssTransform;
		BadAssTransform.Position = glm::vec3(0.f, 2.5f, 0.f);
		BadAssTransform.Scale = glm::vec3(0.5f);
		BadAssTransform.Rotation = glm::angleAxis(Time, glm::vec3(1.f, 0.f, 0.f));

		Transform BadAssTransform1;
		BadAssTransform1.Position = glm::vec3(-2.5f, 0.f, 0.f);
		BadAssTransform1.Scale = glm::vec3(0.5f);
		BadAssTransform1.Rotation = glm::angleAxis(Time, glm::vec3(-1.f, 0.f, 0.f));

	

		RenderCube(BaseTransform);
		RenderCube(BaseTransform * BadAssTransform1);
		RenderCube(BaseTransform * BadAssTransform1 * BabyTransform);
		RenderCube(BaseTransform * BadAssTransform);
		RenderCube(BaseTransform * ChildTransform);
		RenderCube(BaseTransform * ChildTransform * BabyTransform);
		RenderCube(BaseTransform * ChildTransform * BabyTransform * BabyTransform);
	}

	//Wierd Saw Thing

	{

		Transform BaseTransform;
		BaseTransform.Position = glm::vec3(5.f, 1.f, 5.f);
		BaseTransform.Rotation = glm::angleAxis(Time, glm::normalize(glm::vec3(0.5f, -0.f, 1.f)));

		int NumTriangles = 200;
		for (int i = 0; i < NumTriangles; i++)
		{
			float AngleInc = (glm::pi<float>() * 2.f) / NumTriangles;
			float Angle = Time + AngleInc * i;
			float SawAngle = Time * 2.f + AngleInc * i;

			Transform ChildTransform;
			ChildTransform.Position = glm::vec3(2.5f, sin(Time / Angle + i * 0.5f - 2.3f * AngleInc * 3.4f * i * Time / 5.2f - Angle) * cos(AngleInc * -i * -2.2f) * 2.f, cos(AngleInc * i * 2.2f) * 2.f);
			ChildTransform.Scale = glm::vec3(0.2f);

			Transform SpinTransform;
			SpinTransform.Rotation = glm::angleAxis(Angle, glm::vec3(0.f, 1.f, 0.f));

			Transform SawTransform;
			SawTransform.Rotation = glm::angleAxis(SawAngle * 5.f, glm::vec3(0.f, 0.f, 1.f));

			RenderCube(BaseTransform);
			RenderCube(BaseTransform * SpinTransform * ChildTransform * SawTransform);
			
		}



	}

	{

		Transform CurrentTransform;
		CurrentTransform.Position = glm::vec3(-5.f, 5.f, 2.f);
		RenderCube(CurrentTransform);

		int Iterations = 23;
		for (int i  = 0; i < Iterations; ++i)
		{
			Transform Delta;
			Delta.Position = glm::vec3(-1.5, 1.f, 0.f);
			Delta.Scale = glm::vec3(0.8f);
			Delta.Rotation = glm::angleAxis(sin(-Time + i * 0.4f) * 0.4f, glm::vec3(0.f, 0.f, 1.f));

			Transform TwirlDelta;
			TwirlDelta.Rotation = glm::angleAxis(sin(-Time + i * 0.4f) * 0.4f, glm::vec3(0.f, 1.f, 0.f));

			Transform TwistDelta;
			TwistDelta.Rotation = glm::angleAxis(sin(Time * 0.4f + i * 0.21f) * 0.12f, glm::vec3(1.f, 0.f, 1.f));

			CurrentTransform = CurrentTransform * TwistDelta * TwirlDelta *  Delta;
			
			RenderCube(CurrentTransform);
		}

	

	}


	{

		glm::vec3 RotationAxis = glm::vec3(cos(Time * 0.062f), sin(Time * 0.9f) * 2.f, cos(-Time * 0.5f));
		RotationAxis = glm::normalize(RotationAxis);

		Transform BaseTransform;
		BaseTransform.Position = glm::vec3(3.f, 2.f, 20.f);
		BaseTransform.Rotation = glm::angleAxis(Time, RotationAxis);

		RenderCube(BaseTransform);

		Transform BaseInverse = BaseTransform.Inverse();
		BaseInverse.Position = glm::vec3(0.f);



		static Transform ChildTransform = Transform(glm::vec3(2.5f, 0.f, 0.f), quat_identity, glm::vec3(0.5f));


	/*	ChildTransform.Position = glm::vec3(2.5f, 0.f, 0.f);
		ChildTransform.Rotation = glm::angleAxis(Time, glm::vec3(0.f, 1.f, 0.f));
		ChildTransform.Scale = glm::vec3(0.5f);*/

		static bool AttachedToCamera = false;

		if (IsKeyPressed(GLFW_KEY_SPACE))
		{
			if (!AttachedToCamera)
			{
				//Trans cube from box to camera
				//ChildTransform = TheCamera.GetTransform().Inverse() * BaseTransform * ChildTransform;
				Transform WorldSpace = BaseTransform * ChildTransform;
				Transform CameraSpace = TheCamera.GetTransform().Inverse() * WorldSpace;
				ChildTransform = CameraSpace;

				AttachedToCamera = true;
			}
			/*Transform CameraLocal;
			CameraLocal.Position = glm::vec3(0.f, 0.f, -1.5f);
			CameraLocal.Scale = glm::vec3(0.5f);

			Transform AttachWorld = TheCamera.GetTransform() * CameraLocal;
			ChildTransform = BaseTransform.Inverse() * AttachWorld;*/


			/*glm::vec3 WorldPosition = TheCamera.Position + TheCamera.GetForwardVector() * 1.5f;
			ChildTransform.Position = BaseTransform.InverseTransformPosition(WorldPosition);
			ChildTransform.Rotation = BaseTransform.InverseTransformRotation(quat_identity);*/

			RenderCube(TheCamera.GetTransform() * ChildTransform);
		}
		else
		{
			if (AttachedToCamera)
			{
				//trans from camera to box

				Transform WorldSpace = TheCamera.GetTransform() * ChildTransform;
				Transform BoxSpace = BaseTransform.Inverse() * WorldSpace;

				ChildTransform = BoxSpace;

				AttachedToCamera = false;
			}

		RenderCube(BaseTransform * ChildTransform);
		}


	}

	RenderCube(Transform(glm::vec3(0.f, 0.f, -10.f), quat_identity, glm::vec3(20.f, 10.f, 0.f)));
	RenderCube(Transform(glm::vec3(-20.f, -1.f, -10.f)));
	
	RenderSkybox();
	RenderLightSource(MainLight);
	RenderLightSource(SecondLight);

	//Render Player
	if (RenderPlayer)
	{
	RenderCube(Transform(TheCamera.Position));

	}
	
}

int main()
{
	// Initialize GLFW, create a window and context

	glfwInit();

	GLFWwindow* Window;
	Window = glfwCreateWindow(ScreenWidth, ScreenHeight, "This is OpenGl", nullptr, nullptr);




	glfwSetKeyCallback(Window, OnKeyEvent);
	glfwSetCursorPosCallback(Window, OnCursorMove);
	glfwSetMouseButtonCallback(Window, OnMouseButton);
	glfwMakeContextCurrent(Window);
	glfwSwapInterval(0);
	//After the context is bound , we can import the OpenGL extension, through the extension wrangler

	glewInit();

	//Vertex Stuff//
	// Make the Quad//
	
	float QuadData[] = { -0.5f, -0.5f, 0.f,		0.f, 0.f, 1.f,		0.f, 0.f,
						 0.5f, -0.5f, 0.f,		0.f, 0.f, 1.f,		1.f, 0.f,
						-0.5f, 0.5f, 0.f,		0.f, 0.f, 1.f,		0.f, 1.f,
						 0.5f, 0.5f, 0.f,		0.f, 0.f, 1.f,		1.f, 1.f,
	};

	QuadMesh.LoadVerts(QuadData, sizeof(QuadData), nullptr, 0);

	
	CubeMesh.LoadVerts(CubeVertexData, sizeof(CubeVertexData), CubeIndexData, sizeof(CubeIndexData));
	

	float TriData[] = { -0.5f, -0.5f, 0.0f,		0.f, 0.f, 1.f, 	0.f, 0.f,
						 0.5f, -0.5f, 0.0f,		0.f, 0.f, 1.f, 	1.f, 0.f,
						 0.0f, 0.5f, 0.0f,		0.f, 0.f, 1.f,	0.5f, 1.f,
	};

	TriangleMesh.LoadVerts(TriData, sizeof(TriData), nullptr, 0);


	DefaultMaterial.LoadFiles("res/Shader/default.vert", "Res/Shader/default.frag");
	DefaultMaterial.Set("u_DirLight.ShadowBuffer", 2);

	Material PostProcessMaterial;
	PostProcessMaterial.LoadFiles("Res/Shader/post_process.vert", "Res/Shader/post_process.frag");
	PostProcessMaterial.Set("u_FrameColor", 0);
	PostProcessMaterial.Set("u_FrameDepth", 1);

	CherylTexture.LoadFile("Res/Capture2.PNG");
	CherylTexture.Bind();

	SkyboxMaterial.LoadFiles("Res/Shader/Skybox.vert", "Res/Shader/Skybox.frag");
	LightSourceMaterial.LoadFiles("Res/Shader/light_source.vert", "Res/Shader/light_source.frag");
	SkyboxTexture.LoadFiles(SkyboxImages);


	float Ratio = (float)ScreenWidth / ScreenHeight;
	float OrthoSize = 2.f;


	TheCamera.Position = glm::vec3(0.f, 4.f, 8.5f);

	glm::vec3 LightDirection = glm::normalize(glm::vec3(-1.f, -4.4f, -15.f));
	DefaultMaterial.Set("u_DirLight.Direction", LightDirection);
	DefaultMaterial.Set("u_DirLight.Color", glm::vec3(0.9f, 0.2f, 0.2f));

	//Point Light Stuff
	MainLight.Position = glm::vec3(0.f, 1.f, 0.f);
	MainLight.Color = glm::vec3(0.2f, 0.2f, 0.9f);
	MainLight.Radius = 10.f;
	MainLight.UploadToMaterial(0, DefaultMaterial);

	SecondLight.Position = glm::vec3(0.f, 1.f, 0.f);
	SecondLight.Color = glm::vec3(0.2f, 0.2f, 0.9f);
	SecondLight.Radius = 10.f;
	SecondLight.UploadToMaterial(1, DefaultMaterial);

	FlashLight.Position = glm::vec3(0.f, 2.f, 10.f);
	FlashLight.Direction = glm::vec3(0.f, 0.f, -1.f);
	FlashLight.Color = glm::vec3(0.2f, 1.f, 0.3f);
	FlashLight.Angle = glm::radians(20.f);
	FlashLight.Length = 30.f;
	FlashLight.UploadToMatrerial(DefaultMaterial);

	Material BlurVertical;
	Material BlurHorizontal;
	GaussisMaterial.LoadFiles("Res/Shader/horizontalBlurVertex.vert", "Res/Shader/blurFragmentGauss.frag");
	GaussisMaterial.LoadFiles("Res/Shader/verticalBlurVertex.vert", "Res/Shader/blurFragmentGauss.frag");
	GaussisMaterial.Set("t_TargetHeight", (float)ScreenHeight);
	GaussisMaterial.Set("t_TargetWidth", (float)ScreenWidth);

	DefaultMaterial.Set("u_PointLight.Position", glm::vec3(0.f, 2.f, 0.f));
	DefaultMaterial.Set("u_PointLight.Color", glm::vec3(1.f, 0.55f, 0.f));
	DefaultMaterial.Set("u_PointLight.Radius", 10.f);

	glm::mat4 Projection;
	Projection = glm::perspective(glm::radians(60.f), Ratio, 0.2f, 150.f);

	DefaultMaterial.Set("u_Projection", Projection);

	glEnable(GL_DEPTH_TEST);
	float LastFrameTime = 0.f;

	double LastMouseX = 0.0;
	double LastMouseY = 0.0;

	FrameBuffer LightBuffer;
	LightBuffer.Create(4096, 4096);

	FrameBuffer BlurView;
	BlurView.Create(2000, 2000);

	FrameBuffer FB;
	FB.Create(2000, 2000);

	glm::mat4 LightProjection;
	glm::mat4 LightView;
	LightProjection = glm::ortho(-25.f, 25.f, -25.f, 25.f, -30.f, 30.f);
	LightView = glm::lookAt(glm::vec3(0.f), LightDirection, glm::vec3(0.f, 1.f, 0.f));
	


	


	//Main loop of the program
	while (!glfwWindowShouldClose(Window) && !ShouldQuit)
	{
		DefaultMaterial.Use();

		float CurrentTime = glfwGetTime();
		float DeltaTime = CurrentTime - LastFrameTime;
		LastFrameTime = CurrentTime;

		double MouseDeltaX = MouseX - LastMouseX;
		double MouseDeltaY = MouseY - LastMouseY;
		LastMouseX = MouseX;
		LastMouseY = MouseY;

		if (ActivateGauissBluri && IsKeyPressed(GLFW_KEY_G))
		{

		}

		if (CameraMovementActive)
		{

		TheCamera.AddYaw(-MouseDeltaX * 0.2f);
		TheCamera.AddPitch(-MouseDeltaY * 0.2f);
		

		glm::vec3 CameraMovement = glm::vec3(0.f);
		if (IsKeyPressed(GLFW_KEY_W))
			CameraMovement += TheCamera.GetForwardVector();
		if (IsKeyPressed(GLFW_KEY_S))
			CameraMovement += -TheCamera.GetForwardVector();
		if (IsKeyPressed(GLFW_KEY_A))
			CameraMovement += -TheCamera.GetRightVector();
		if (IsKeyPressed(GLFW_KEY_D))
			CameraMovement += TheCamera.GetRightVector();
		if (IsKeyPressed(GLFW_KEY_Z))
			CameraMovement += glm::vec3(0.f, 1.f, 0.f);
		if (IsKeyPressed(GLFW_KEY_X))
			CameraMovement += glm::vec3(0.f, -1.f, 0.f);
		if (IsKeyPressed(GLFW_KEY_E))
		{
			TheCamera.AddYaw(-45.f * DeltaTime);
		}
		if (IsKeyPressed(GLFW_KEY_Q))
		{
			TheCamera.AddYaw(45.f * DeltaTime);
		}
		
		TheCamera.Position += CameraMovement * 5.f * DeltaTime;
		
		}
	    DefaultMaterial.Set("u_View", TheCamera.GetViewMatrix());
		DefaultMaterial.Set("u_EyePosition", TheCamera.Position);
		

		MainLight.Color = glm::vec3(1.f, 0.3f, 0.3f) * (glm::sin(CurrentTime * 5.f) + 1.f) * 4.f;
		MainLight.Position = glm::vec3(glm::sin(CurrentTime) * 5.f, 0.2f, glm::cos(CurrentTime) * 2.f);
		MainLight.UploadToMaterial(0, DefaultMaterial);

		SecondLight.Color = glm::vec3(0.2f, 0.2f, 0.9f) * (glm::sin(CurrentTime * 5.f) + 1.f) * 4.f;;
		SecondLight.Position = glm::vec3(glm::sin(CurrentTime) * -5.f, 0.2f, glm::cos(CurrentTime) * 2.f);;
		SecondLight.UploadToMaterial(1, DefaultMaterial);

		if (IsKeyPressed(GLFW_KEY_F))
		{
			FlashLight.Position = TheCamera.Position;
			FlashLight.Direction = TheCamera.GetForwardVector();
			FlashLight.UploadToMatrerial(DefaultMaterial);
		}

		//Rotate the cube

		//glm::quat DeltaRotation = glm::angleAxis(glm::radians(90.f) * DeltaTime, glm::normalize(glm::vec3(1.f, 1.f, 0.2f)));
		//Transforms[0].Rotation *= DeltaRotation;

		//clear the screen

		DefaultMaterial.Set("u_Projection", LightProjection);
		DefaultMaterial.Set("u_View", LightView);

		LightSourceMaterial.Set("u_Projection", LightProjection);
		LightSourceMaterial.Set("u_View", LightView);
		
		LightBuffer.Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		RenderScene(true);

		LightBuffer.UnBind();
		//RENDER TO FRAMBUFFER


		DefaultMaterial.Set("u_Projection", Projection);
		DefaultMaterial.Set("u_View", TheCamera.GetViewMatrix());
		DefaultMaterial.Set("u_DirLight.ViewProjection", LightProjection* LightView);

		LightSourceMaterial.Set("u_Projection", Projection);
		LightSourceMaterial.Set("u_View", TheCamera.GetViewMatrix());

		SkyboxMaterial.Set("u_Projection", Projection);
		SkyboxMaterial.Set("u_View", TheCamera.GetViewMatrix());


		LightBuffer.DepthTexture.Bind(2);

		FB.Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//DefaultMaterial.Set("u_View", glm::lookAt(glm::vec3(5.f, 5.f, 5.f), TheCamera.Position, glm::vec3(0.f, 1.f, 0.f)));
		RenderScene(false);
		FB.UnBind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//RENDER TO SCREEN
	    //DefaultMaterial.Set("u_View", TheCamera.GetViewMatrix());
		//RenderScene(false);
		glActiveTexture(GL_TEXTURE0);
		FB.ColorTexture.Bind();
		//RenderQuad(Transform(glm::vec3(0.f, 20.f, -20.f), quat_identity, glm::vec3(45.f)));

		CherylTexture.Bind();
		//RenderQuad(Transform(glm::vec3(6.f, 0.f, 5.f), quat_identity, glm::vec3(5.f)));

		GaussisMaterial.Use();
		FB.Bind();

		GLenum StatusForFBs = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (StatusForFBs == GL_FRAMEBUFFER_COMPLETE)
		{
			printf("STATUS IS SECURED");

		}

		FB.UnBind();


		PostProcessMaterial.Use();
		FB.ColorTexture.Bind(0);
		FB.DepthTexture.Bind(1);
		RenderFullScreenQuad();

		//float SinTime = sin(glfwGetTime() * 5.f) * 5.5f + 2.5f;

		//Swap the back buffer to the front and poli and handle window events
		glfwSwapBuffers(Window);
		glfwPollEvents();

	}

	return 0;

}
