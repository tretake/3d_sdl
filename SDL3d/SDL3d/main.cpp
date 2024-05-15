#include <iostream>
#include <SDL.h>



SDL_Window* janela;
SDL_Renderer* render;
SDL_Event e;

float janelaL = 300;
float jld2 = janelaL / 2;
float janelaA = 300;
float jad2 = janelaA / 2;
void iniciar()
{
	SDL_Init(SDL_INIT_VIDEO);
	janela = SDL_CreateWindow("3d", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, janelaL, janelaA, SDL_WINDOW_RESIZABLE);
	render = SDL_CreateRenderer(janela, -1, SDL_RENDERER_PRESENTVSYNC);
}
void desligar()
{
	SDL_DestroyRenderer(render);
	SDL_DestroyWindow(janela);
	SDL_Quit();
}

struct vf3d
{
	float x;
	float y;
	float z;
};

struct jogador
{
	float x;
	float y;
	float z;
};
vf3d velocidade = { 0,0,0 };
float rotacaoz = 0;
float rotacaox = 0;
float angulo_de_visao = 90.0f;

jogador p1 = { 0,0,0 };



vf3d trasformacao_de_ponto(vf3d v)
{
	vf3d Rv;

	Rv.z = v.z - p1.z;
	float distancia_da_borda = tan((angulo_de_visao / 2) * (3.1415f / 180.0f)) * (Rv.z);
	Rv.x = { (v.x / distancia_da_borda) * jld2 + jld2 };
	Rv.y = { (v.y / distancia_da_borda) * jad2 + jad2 };
	return Rv;
}

vf3d rodar_pontoz(vf3d ponto , vf3d centro , float angulo)
{
	vf3d np = 
	{ (ponto.x - centro.x)*cos(angulo) -sin(angulo)*(ponto.y - centro.y) + centro.x ,
	  (ponto.x - centro.x)*sin(angulo) +cos(angulo)*( ponto.y - centro.y) +centro.y,
	  ponto.z 
	};

	return np;
}
vf3d rodar_pontox(vf3d ponto, vf3d centro, float angulo)
{
	vf3d np =
	{ ponto.x ,
	  (ponto.y - centro.y) * cos(angulo) - sin(angulo) * (ponto.z - centro.z) + centro.y,
	   (ponto.y - centro.y)* sin(angulo) + cos(angulo) * (ponto.z - centro.z) + centro.z
	};

	return np;
}

struct quadrado
{
	vf3d v[4];

	void desenhar()
	{

		SDL_SetRenderDrawColor(render, 0xFF, 0xFF, 0xFF, 0xFF);
		vf3d Rv[4];
		for (int i = 0; i < 4; i++)
		{
			Rv[i] = trasformacao_de_ponto(v[i]);
			SDL_FRect vertice = { Rv[i].x - 5 ,Rv[i].y - 5,10,10 };
			SDL_RenderFillRectF(render, &vertice);
			std::cout << "v" << i <<": " << Rv[i].x << "," << Rv[i].y << std::endl;
		}
		SDL_SetRenderDrawColor(render, 0xFF, 0x00, 0x00, 0xFF);
		SDL_RenderDrawLine(render, Rv[0].x, Rv[0].y, Rv[1].x, Rv[1].y);
		SDL_RenderDrawLine(render, Rv[0].x, Rv[0].y, Rv[2].x, Rv[2].y);
		SDL_RenderDrawLine(render, Rv[2].x, Rv[2].y, Rv[3].x, Rv[3].y);
		SDL_RenderDrawLine(render, Rv[3].x, Rv[3].y, Rv[1].x, Rv[1].y);

		SDL_RenderDrawLine(render, Rv[0].x, Rv[0].y, Rv[3].x, Rv[3].y);
		SDL_RenderDrawLine(render, Rv[2].x, Rv[2].y, Rv[1].x, Rv[1].y);
	}

};

struct cubo
{
	vf3d centro;
	quadrado faces[2];

	cubo(vf3d c, float d)
	{
		centro = c;
		faces[0] =
		{ c.x - d,c.y - d,c.z - d,
		  c.x + d,c.y - d,c.z - d,
		  c.x - d,c.y + d,c.z - d,
		  c.x + d,c.y + d,c.z - d
		};
		faces[1] =
		{ c.x - d,c.y - d,c.z + d,
		  c.x + d,c.y - d,c.z + d,
		  c.x - d,c.y + d,c.z + d,
		  c.x + d,c.y + d,c.z + d
		};
	}

	void desenhar()
	{
		faces[0].desenhar();
		faces[1].desenhar();

		SDL_SetRenderDrawColor(render, 0xFF, 0x00, 0x00, 0xFF);
		quadrado Rfaces[2];
		
		for(int i= 0;i<2;i++)
			for (int h = 0; h < 4; h++)
			{
				Rfaces[i].v[h] = trasformacao_de_ponto(faces[i].v[h]);
			}

		SDL_RenderDrawLineF(render,Rfaces[0].v[0].x ,Rfaces[0].v[0].y, Rfaces[1].v[1].x, Rfaces[1].v[1].y);
		SDL_RenderDrawLineF(render, Rfaces[0].v[1].x, Rfaces[0].v[1].y, Rfaces[1].v[0].x, Rfaces[1].v[0].y);

		SDL_RenderDrawLineF(render, Rfaces[0].v[2].x, Rfaces[0].v[2].y, Rfaces[1].v[3].x, Rfaces[1].v[3].y);
		SDL_RenderDrawLineF(render, Rfaces[0].v[3].x, Rfaces[0].v[3].y, Rfaces[1].v[2].x, Rfaces[1].v[2].y);

		SDL_RenderDrawLineF(render, Rfaces[0].v[2].x, Rfaces[0].v[2].y, Rfaces[1].v[0].x, Rfaces[1].v[0].y);
		SDL_RenderDrawLineF(render, Rfaces[0].v[0].x, Rfaces[0].v[0].y, Rfaces[1].v[2].x, Rfaces[1].v[2].y);

		SDL_RenderDrawLineF(render, Rfaces[0].v[3].x, Rfaces[0].v[3].y, Rfaces[1].v[1].x, Rfaces[1].v[1].y);
		SDL_RenderDrawLineF(render, Rfaces[0].v[1].x, Rfaces[0].v[1].y, Rfaces[1].v[3].x, Rfaces[1].v[3].y);
	
	}

	void rotacao_eixo_z(float angulo)
	{
		for(int i = 0 ; i< 2 ; i++)
			for (int j = 0; j < 4; j++)
			{
				faces[i].v[j] = rodar_pontoz(faces[i].v[j], centro, angulo);
			}
	}
	void rotacao_eixo_x(float angulo)
	{
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 4; j++)
			{
				faces[i].v[j] = rodar_pontox(faces[i].v[j], centro, angulo);
			}
	}

};





int main(int argc, char** argv)
{
	iniciar();
	
	cubo qq({ 0,0,0 }, 20);
	

	bool rodando = true;
	while (rodando)
	{
		SDL_SetRenderDrawColor(render, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderClear(render);

		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_KEYDOWN:
				switch (e.key.keysym.sym)
				{
				case SDLK_UP:
					velocidade.z =- 2;
					break;
				case SDLK_DOWN:
					velocidade.z = 2;
					break;
				case SDLK_a:
					rotacaoz = 0.01f;
					break;
				case SDLK_d:
					rotacaoz = -0.01f;
					break;
				case SDLK_w:
					rotacaox = 0.01f;
					break;
				case SDLK_s:
					rotacaox = -0.01f;
					break;
				case SDLK_RIGHT:

					break;
				case SDLK_b:
					rodando = false;
					break;
				}
				break;
			case SDL_KEYUP:
				switch (e.key.keysym.sym)
				{
				case SDLK_UP:
					velocidade.z = 0;
					break;
				case SDLK_DOWN:
					velocidade.z = 0;
					break;
				case SDLK_a:
					rotacaoz = 0;
					break;
				case SDLK_d:
					rotacaoz = 0;
					break;
				case SDLK_w:
					rotacaox = 0;
					break;
				case SDLK_s:
					rotacaox = 0;
					break;
				case SDLK_LEFT:

					break;
				case SDLK_RIGHT:

					break;
				case SDLK_b:
					rodando = false;
					break;
				}
				break;
			}
		}
		p1.z += velocidade.z;


			qq.rotacao_eixo_x(rotacaox);
			qq.rotacao_eixo_z(rotacaoz);

		qq.desenhar();

		


		



		//ss.desenhar();

		

		SDL_RenderPresent(render);
		
	}
	desligar();
	return 1;
}