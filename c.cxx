#include "network.h"
#include "SDL.h"
#include <stdlib.h>
#include <stdio.h>
int main( int argc, char** argv)
	{
		NetworkClient Client;
		NetworkServer Server;
		if( argc >= 2 ) 
			{
				Client.OpenSocket( argv[1], 5555 );
			}
		else
			{
				Server.OpenSocket( 5555 );
			}
		Initiate Init;
		Input Input;
		Init.Start();
		Init.CreateWindow( "Pong", 800, 600, 0 );
		Init.CreateRenderer();	
		Display paddle1;
		Display paddle2;
		Display ball;
		Music Theme;
		Theme.Load( "Tetris.ogg" );
		paddle1.Load( Init.Renderer ,"1.png" );
		paddle2.Load( Init.Renderer ,"1.png" );
		ball.Load( Init.Renderer ,"1.png" );
		Theme.Play( 1 );
		while( !Input.Quit )
			{
				Input.Get();
				static int ballx = 400, bally = 300, speedX = 5, speedY = 0;
				int MouseY2, Paddle1 = Input.MouseY-50, Paddle2 = MouseY2;
				SDL_SetRenderDrawColor( Init.Renderer, 0, 0, 0, 255 );	
				SDL_RenderClear( Init.Renderer );
				if( argc == 2 )
					{
						char MouseY[255];
						snprintf(MouseY, sizeof(MouseY), "%d", Input.MouseY);
						Client.Recive();
						Client.Send( MouseY );
						sscanf(Client.buffer, "%d %d %d", &MouseY2, &ballx, &bally);
						paddle1.Draw( Init.Renderer, 780, Input.MouseY-50, 20, 100 );
						paddle2.Draw( Init.Renderer, 0, MouseY2-50, 20, 100 );
						ball.Draw( Init.Renderer, ballx, bally, 20, 20 );
					}
				else
					{
						char MouseY[255];
						snprintf(MouseY, sizeof(MouseY), "%d %d %d", Input.MouseY, ballx, bally);
						Server.Send( MouseY );
						Server.Recive();
						sscanf(Server.buffer, "%d", &MouseY2 );
						if( ballx >= 0 && ballx <= 20 && bally <= Paddle1+50 && bally >= Paddle1-70 )
								{
									speedX = 5;
									speedY = -(((Paddle1) - bally) /10);
								}
						if( ballx <= 800  &&ballx >= 760 && bally <= Paddle2+50 && bally >= Paddle2-70 )
								{
									speedX = -5;
									speedY = -(((Paddle2) - bally) /10);
								}
						if( ballx <= 0 || ballx >= 800 )
								{
									ballx = 400;
									bally = 300;
								}
						if( bally < 20 || bally > 580)
							{
								speedY = -speedY;
							}
						ballx = ballx + speedX;
						bally = bally + speedY;
						paddle1.Draw( Init.Renderer, 0, Input.MouseY-50, 20, 100 );
						paddle2.Draw( Init.Renderer, 780, MouseY2-50, 20, 100 );
						ball.Draw( Init.Renderer, ballx, bally, 20, 20 );
					}
																																																																										
				SDL_RenderPresent( Init.Renderer );
			}
		paddle1.Clear();
		paddle2.Clear();
		ball.Clear();
		Init.Clear();
		return 0;
	}
