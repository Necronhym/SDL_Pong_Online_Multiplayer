/*

Compiler Note:
g++ <name.cxx> -o <name> -lSDL2 -lSDL2_image -lSDL2_mixer

*/
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
//A class to handle SDL initiation for aduio and video, creation of a window and renderer

class Initiate
	{
		public:
		/*
		Variables
		Window
		Renderer
		*/
		SDL_Window *Window;
		SDL_Renderer *Renderer;
		SDL_Joystick *Joystick;
		/*
			Initiates SDL Video, audio and joystick. Opens sound ports and initiates joystick
		*/
		void Start()
			{
				SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK );
				IMG_Init( IMG_INIT_PNG );
				Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 );	
				if(	SDL_NumJoysticks() != 0 )
					{
						SDL_Joystick *joystick = SDL_JoystickOpen(0);
					}
			
			}
		/*
			Creates a window 
			Teskes Window name, and dimentions
		*/
		void CreateWindow( const char* Name, int Width, int Height, bool Fullscreen = 0)
			{
				if( Fullscreen )
					{
						Window = SDL_CreateWindow( Name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Width, Height, SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL );
					}
				else
					{
						Window = SDL_CreateWindow( Name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Width, Height, SDL_WINDOW_OPENGL );
					}
			}
		/*
			Creates a renderer
		*/
		void CreateRenderer()
			{
				Renderer = SDL_CreateRenderer( Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			}
		/*
			Destorys window and renderer to clear up memeory
		*/
		void Clear()
			{
				SDL_DestroyWindow( Window );
				SDL_DestroyRenderer( Renderer );
			}
	};

//A class to handle the visual display of a single object:

class Display
	{
		/*
			SDL Variables
			Loaded Image
			Texture conversion
			Image location
		*/
		SDL_Surface *image;
		SDL_Texture *image_texture;
		SDL_Rect texture_destination;
		public:
		/*
			Loads Image call outside of loop to avoid memory flooding
			Takes renderer pointer and loaded file name
		*/
		void Load( SDL_Renderer* Renderer, const char* File )
			{
				image = IMG_Load( File );
				image_texture = SDL_CreateTextureFromSurface( Renderer, image );
				SDL_FreeSurface( image );
			}
		/*
			Displays image ( call inside of loop )
			Takes renderer pointer, image coordinates and dimentions
		*/
		void Draw( SDL_Renderer* Renderer, int x, int y, int w, int h, int r = 255, int g = 255, int b = 255, int a = 255 )
			{
				SDL_SetRenderDrawColor( Renderer ,r, g, b, a );
				texture_destination.x = x;
				texture_destination.y = y;
				texture_destination.w = w;
				texture_destination.h = h;
				SDL_RenderCopy( Renderer, image_texture, NULL, &texture_destination );
			}
		/*
			Clears the testure buffer ( call outside of loop when done with texture );
		*/
		void Clear()
			{
				SDL_DestroyTexture( image_texture );
			}
	};

//A class to handle the Music playback ( > 10sec )

class Music
	{
		/*
			SDL_Variableis
			Music file
		*/
		Mix_Music* Music;
		public:
		/*
			Loads a music file, ball before loop to avoid memory flooding
			Tales argument File name and location
		*/
		void Load( const char* File )
			{
				Music = Mix_LoadMUS( File );
			}
		/*
			Plays Music file
			Takes loop cariable( determens if played once or in a loop )
		*/
		void Play( bool loop = 0 )
			{
				switch (loop)
					{
						case 1:
						Mix_PlayMusic( Music, -1 );
						break;
						default:
						Mix_PlayMusic( Music, 1 );
						break;

					}
			}
		/*
			Stops The Music
		*/
		void Stop()
			{
				Mix_HaltMusic();
			}
		/*
			Clears the music buffer
		*/
		void Clear()
			{
				Mix_FreeMusic( Music );
			}
	};

//A class to handle sound playback ( < 10sec )

class Sound
	{
		/*
			SDL_Variable
			Sound file Must Be WAV format
		*/
		Mix_Chunk* Sound;
		public:
		/*
			Loads a WAV format in to memeory call before loop to avoid memory flooding
			Takes argument File name and location
		*/
		void Load( const char* File )
			{
				Sound = Mix_LoadWAV( File );	
			}
		/*
			Plays music file
			Takes variable number of times to play ( -1 loop forever )
		*/
		void Play( int loop )
			{
				Mix_PlayChannel( -1, Sound, loop );
			}
		/*
			Stops sound
		*/
		void Stop()
			{
				Mix_HaltChannel(-1);
			}
		/*
			Clears Memory after being done with sound
		*/
		void Clear()
			{
				Mix_FreeChunk( Sound );
			}
	};
// A class to handle window, mouse, keyboard and joystick input
class Input
	{
		SDL_Event event;
		public:
		/*
			Variables
			Quite event boolean
			Mouse location
			Mouse buttons array [ Left, Middle, Right ]
		*/
		bool Quit = false;
		int MouseX, MouseY;
		bool MouseButton[2] = { 0 };
		/*
			Gets mouse location and buttons states
		*/
		void Mouse()
			{
				//Gets mouse location
				SDL_GetMouseState( &MouseX, &MouseY );
				//Checks from button press and relese
				switch( event.type )
					{
						//Button Press
						case ( SDL_MOUSEBUTTONDOWN ):
							//Left Button
							if( event.button.button == SDL_BUTTON_LEFT )
								{
									MouseButton[0] = 1;
								}
							//Middle Button
							if( event.button.button == SDL_BUTTON_MIDDLE )
								{
									MouseButton[1] = 1;
								}
							//Right Button
							if( event.button.button == SDL_BUTTON_RIGHT )
								{
									MouseButton[2] = 1;
								}
						break;
						//Bittpm Relese
						case ( SDL_MOUSEBUTTONUP ):
							//Left Button
							if( event.button.button == SDL_BUTTON_LEFT )
								{
									MouseButton[0] = 0;
								}
							//Middle Button
							if( event.button.button == SDL_BUTTON_MIDDLE )
								{
									MouseButton[1] = 0;
								}
							//Right Button
							if( event.button.button == SDL_BUTTON_RIGHT )
								{
									MouseButton[2] = 0;
								}		
						break;
					}
			}
		void Keyboard()
			{
			
			}
		void Joystick()
			{

			}
		/*
			Checks for input events:
		*/
		void Get()
			{
				while( SDL_PollEvent( &event ) )
					{
						if( event.type == SDL_QUIT )
							{
								Quit = true;
							}
						Mouse();
					}
			}
	};
