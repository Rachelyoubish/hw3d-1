#include "App.h"
#include <algorithm>
#include "ChiliMath.h"
#include "imgui/imgui.h"
#include "ChiliUtil.h"
#include "Testing.h"
#include "PerfLog.h"
#include "TestModelProbe.h"
#include "Testing.h"
#include "Camera.h"

namespace dx = DirectX;
static float temp{};

App::App( const std::string& commandLine )
	:
	commandLine( commandLine ),
	wnd( 1280,720,"DirectX 11 Fun Time" ),
	scriptCommander( TokenizeQuoted( commandLine ) ),
	light( wnd.Gfx() )
{
	cameras.AddCamera( std::make_unique<Camera>( wnd.Gfx(),"A",dx::XMFLOAT3{ -13.5f,6.0f,3.5f },0.0f,PI / 2.0f ) );
	cameras.AddCamera( std::make_unique<Camera>( wnd.Gfx(),"B",dx::XMFLOAT3{ -13.5f,28.8f,-6.4f },PI / 180.0f * 13.0f,PI / 180.0f * 61.0f ) );

	//cube.SetPos( { 4.0f,0.0f,0.0f } );
	//cube2.SetPos( { 0.0f,4.0f,0.0f } );
	//nano.SetRootTransform(
	//	dx::XMMatrixRotationY( PI / 2.f ) *
	//	dx::XMMatrixTranslation( 27.f,-0.56f,1.7f )
	//);

	// Root transformations don't need to be set here, when already updating in actual DoFrame().
	//diablo.SetRootTransform(
	//	dx::XMMatrixRotationX( PI / 2.f ) *
	//	dx::XMMatrixRotationY( PI / 2.f ) *
	//	dx::XMMatrixTranslation( 15.f, 1.f, 0.f )
	//);
	//dxbox.SetRootTransform( 
	//	dx::XMMatrixRotationX( -PI / 2.f ) *
	//	dx::XMMatrixRotationY( -PI / 2.f ) *
	//	dx::XMMatrixTranslation( 0.f, 10.f, 0.f ) 
	//);
	
	// cube.LinkTechniques( rg );
	// cube2.LinkTechniques( rg );
	light.LinkTechniques( rg );
	// sponza.LinkTechniques( rg );
	// nano.LinkTechniques( rg );
	// dxbox.LinkTechniques( rg );
	diablo.LinkTechniques( rg );
	cameras.LinkTechniques( rg );

	// Imgui theme stuff. 
	{
		ImVec4 *colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_Text] = ImVec4( 1.00f, 1.00f, 1.00f, 1.00f );
		colors[ImGuiCol_TextDisabled] = ImVec4( 0.50f, 0.50f, 0.50f, 1.00f );
		colors[ImGuiCol_WindowBg] = ImVec4( 0.06f, 0.06f, 0.06f, 0.94f );
		colors[ImGuiCol_ChildBg] = ImVec4( 1.00f, 1.00f, 1.00f, 0.00f );
		colors[ImGuiCol_PopupBg] = ImVec4( 0.08f, 0.08f, 0.08f, 0.94f );
		colors[ImGuiCol_Border] = ImVec4( 0.43f, 0.43f, 0.50f, 0.50f );
		colors[ImGuiCol_BorderShadow] = ImVec4( 0.00f, 0.00f, 0.00f, 0.00f );
		colors[ImGuiCol_FrameBg] = ImVec4( 0.20f, 0.21f, 0.22f, 0.54f );
		colors[ImGuiCol_FrameBgHovered] = ImVec4( 0.40f, 0.40f, 0.40f, 0.40f );
		colors[ImGuiCol_FrameBgActive] = ImVec4( 0.18f, 0.18f, 0.18f, 0.67f );
		// og: colors[ImGuiCol_TitleBg] = ImVec4( 0.04f, 0.04f, 0.04f, 1.00f );
		colors[ImGuiCol_TitleBg] = ImVec4( 0.20f, 0.70f, 0.13f, 1.00f );
		// og: colors[ImGuiCol_TitleBgActive] = ImVec4( 0.29f, 0.29f, 0.29f, 1.00f );
		colors[ImGuiCol_TitleBgActive] = ImVec4( 0.20f, 0.70f, 0.13f, 1.00f );
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4( 0.00f, 0.00f, 0.00f, 0.51f );
		colors[ImGuiCol_MenuBarBg] = ImVec4( 0.14f, 0.14f, 0.14f, 1.00f );
		colors[ImGuiCol_ScrollbarBg] = ImVec4( 0.02f, 0.02f, 0.02f, 0.53f );
		colors[ImGuiCol_ScrollbarGrab] = ImVec4( 0.31f, 0.31f, 0.31f, 1.00f );
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4( 0.41f, 0.41f, 0.41f, 1.00f );
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4( 0.51f, 0.51f, 0.51f, 1.00f );
		colors[ImGuiCol_CheckMark] = ImVec4( 0.94f, 0.94f, 0.94f, 1.00f );
		colors[ImGuiCol_SliderGrab] = ImVec4( 0.51f, 0.51f, 0.51f, 1.00f );
		colors[ImGuiCol_SliderGrabActive] = ImVec4( 0.86f, 0.86f, 0.86f, 1.00f );
		colors[ImGuiCol_Button] = ImVec4( 0.44f, 0.44f, 0.44f, 0.40f );
		colors[ImGuiCol_ButtonHovered] = ImVec4( 0.46f, 0.47f, 0.48f, 1.00f );
		colors[ImGuiCol_ButtonActive] = ImVec4( 0.42f, 0.42f, 0.42f, 1.00f );
		colors[ImGuiCol_Header] = ImVec4( 0.70f, 0.70f, 0.70f, 0.31f );
		colors[ImGuiCol_HeaderHovered] = ImVec4( 0.70f, 0.70f, 0.70f, 0.80f );
		colors[ImGuiCol_HeaderActive] = ImVec4( 0.48f, 0.50f, 0.52f, 1.00f );
		colors[ImGuiCol_Separator] = ImVec4( 0.43f, 0.43f, 0.50f, 0.50f );
		colors[ImGuiCol_SeparatorHovered] = ImVec4( 0.72f, 0.72f, 0.72f, 0.78f );
		colors[ImGuiCol_SeparatorActive] = ImVec4( 0.51f, 0.51f, 0.51f, 1.00f );
		colors[ImGuiCol_ResizeGrip] = ImVec4( 0.91f, 0.91f, 0.91f, 0.25f );
		colors[ImGuiCol_ResizeGripHovered] = ImVec4( 0.81f, 0.81f, 0.81f, 0.67f );
		colors[ImGuiCol_ResizeGripActive] = ImVec4( 0.46f, 0.46f, 0.46f, 0.95f );
		colors[ImGuiCol_PlotLines] = ImVec4( 0.61f, 0.61f, 0.61f, 1.00f );
		colors[ImGuiCol_PlotLinesHovered] = ImVec4( 1.00f, 0.43f, 0.35f, 1.00f );
		colors[ImGuiCol_PlotHistogram] = ImVec4( 0.73f, 0.60f, 0.15f, 1.00f );
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4( 1.00f, 0.60f, 0.00f, 1.00f );
		colors[ImGuiCol_TextSelectedBg] = ImVec4( 0.87f, 0.87f, 0.87f, 0.35f );
		colors[ImGuiCol_ModalWindowDarkening] = ImVec4( 0.80f, 0.80f, 0.80f, 0.35f );
		colors[ImGuiCol_DragDropTarget] = ImVec4( 1.00f, 1.00f, 0.00f, 0.90f );
		colors[ImGuiCol_NavHighlight] = ImVec4( 0.60f, 0.60f, 0.60f, 1.00f );
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4( 1.00f, 1.00f, 1.00f, 0.70f );
	}
}

void App::HandleInput( float dt )
{
	while( const auto e = wnd.kbd.ReadKey() )
	{
		if( !e->IsPress() )
		{
			continue;
		}

		switch( e->GetCode() )
		{
		case VK_ESCAPE:
			if( wnd.CursorEnabled() )
			{
				wnd.DisableCursor();
				wnd.mouse.EnableRaw();
			}
			else
			{
				wnd.EnableCursor();
				wnd.mouse.DisableRaw();
			}
			break;
		case VK_F1:
			showDemoWindow = true;
			break;
		}
	}

	if( !wnd.CursorEnabled() )
	{
		if( wnd.kbd.KeyIsPressed( 'W' ) )
		{
			cameras->Translate( { 0.0f,0.0f,dt } );
		}
		if( wnd.kbd.KeyIsPressed( 'A' ) )
		{
			cameras->Translate( { -dt,0.0f,0.0f } );
		}
		if( wnd.kbd.KeyIsPressed( 'S' ) )
		{
			cameras->Translate( { 0.0f,0.0f,-dt } );
		}
		if( wnd.kbd.KeyIsPressed( 'D' ) )
		{
			cameras->Translate( { dt,0.0f,0.0f } );
		}
		if( wnd.kbd.KeyIsPressed( 'R' ) )
		{
			cameras->Translate( { 0.0f,dt,0.0f } );
		}
		if( wnd.kbd.KeyIsPressed( 'F' ) )
		{
			cameras->Translate( { 0.0f,-dt,0.0f } );
		}
	}

	while( const auto delta = wnd.mouse.ReadRawDelta() )
	{
		if( !wnd.CursorEnabled() )
		{
			cameras->Rotate( (float)delta->x,(float)delta->y );
		}
	}
}

void App::DoFrame( float dt )
{
	wnd.Gfx().BeginFrame( 0.07f,0.0f,0.12f );
	cameras->BindToGraphics( wnd.Gfx() );
	light.Bind( wnd.Gfx(),cameras->GetMatrix() );

	temp += dt;
	//dxbox.UpdateRootTransform( 
	//	dx::XMMatrixRotationX(-PI / 2.f) *
	//	dx::XMMatrixRotationY( (2.f * temp) / 3.f) *
	//	dx::XMMatrixTranslation( 0.f, 10.f, 0.f ) 
	//);
	diablo.UpdateRootTransform(
		dx::XMMatrixRotationX( PI / 2.f ) *
		dx::XMMatrixRotationY( (2.f * temp) / 3.f ) *
		dx::XMMatrixTranslation( 27.f, 1.f, 0.f )
	);
		
	light.Submit();
	// cube.Submit();
	// sponza.Submit();
	// cube2.Submit();
	// nano.Submit();
	// dxbox.Submit();
	diablo.Submit();
	cameras.Submit();

	rg.Execute( wnd.Gfx() );

	// imgui windows
	static MP sponzeProbe{ "Sponza" };
	static MP nanoProbe{ "Nano" };
	static MP dxboxProbe{ "DxBox" };
	static MP diabloProbe{ "Diablo" };
	// sponzeProbe.SpawnWindow( sponza );
	// nanoProbe.SpawnWindow( nano );
	// dxboxProbe.SpawnWindow( dxbox );
	diabloProbe.SpawnWindow( diablo );
	cameras.SpawnWindow( wnd.Gfx() );
	light.SpawnControlWindow();
	ShowImguiDemoWindow();
	// cube.SpawnControlWindow( wnd.Gfx(),"Cube 1" );
	// cube2.SpawnControlWindow( wnd.Gfx(),"Cube 2" );
	
	rg.RenderWidgets( wnd.Gfx() );

	// present
	wnd.Gfx().EndFrame();
	rg.Reset();
}

void App::ShowImguiDemoWindow()
{
	if( showDemoWindow )
	{
		ImGui::ShowDemoWindow( &showDemoWindow );
	}
}

App::~App()
{}

int App::Go()
{
	while( true )
	{
		// process all messages pending, but to not block for new messages
		if( const auto ecode = Window::ProcessMessages() )
		{
			// if return optional has value, means we're quitting so return exit code
			return *ecode;
		}
		// execute the game logic
		const auto dt = timer.Mark() * speed_factor;
		HandleInput( dt );
		DoFrame( dt );
	}
}