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
	nano.SetRootTransform(
		dx::XMMatrixRotationY( PI / 2.f ) *
		dx::XMMatrixTranslation( 27.f,-0.56f,1.7f )
	);
	diablo.SetRootTransform(
		dx::XMMatrixRotationX( PI / 2.f ) *
		dx::XMMatrixRotationY( PI / 2.f ) *
		dx::XMMatrixTranslation( 15.f, 1.f, 0.f )
	);
	//dxbox.SetRootTransform( 
	//	dx::XMMatrixRotationX( -PI / 2.f ) *
	//	dx::XMMatrixRotationY( -PI / 2.f ) *
	//	dx::XMMatrixTranslation( 0.f, 10.f, 0.f ) 
	//);
	
	// cube.LinkTechniques( rg );
	// cube2.LinkTechniques( rg );
	light.LinkTechniques( rg );
	sponza.LinkTechniques( rg );
	nano.LinkTechniques( rg );
	dxbox.LinkTechniques( rg );
	diablo.LinkTechniques( rg );
	cameras.LinkTechniques( rg );
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
	dxbox.UpdateRootTransform( 
		dx::XMMatrixRotationX(-PI / 2.f) *
		dx::XMMatrixRotationY( (2.f * temp) / 3.f) *
		dx::XMMatrixTranslation( 0.f, 10.f, 0.f ) );
		
	light.Submit();
	// cube.Submit();
	sponza.Submit();
	// cube2.Submit();
	nano.Submit();
	dxbox.Submit();
	diablo.Submit();
	cameras.Submit();

	rg.Execute( wnd.Gfx() );

	// imgui windows
	static MP sponzeProbe{ "Sponza" };
	static MP nanoProbe{ "Nano" };
	static MP dxboxProbe{ "DxBox" };
	sponzeProbe.SpawnWindow( sponza );
	nanoProbe.SpawnWindow( nano );
	dxboxProbe.SpawnWindow( dxbox );
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