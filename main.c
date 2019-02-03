#include <allegro.h>

//declarando funções
void sair();
void control();
int colidir (int Ax, int Ay, int Bx, int By, int Aw, int Ah, int Bw, int Bh);

//Variaveis Gloais
int sai  = 0; //Se é igualada a 0 pois futuramente quando ela for igualada a 1 o programa irá se encerrar
int width  = 740; //Largura da tela
int height  = 700; //Altura da tela
int atirar  = 0 ; //O tiro do jogador começa com valor zero, para ele não iniciar o jogo já atirando

struct obj{//Objetos utilizados como coordenadas
 int x;
 int y;
 int wx;
 int wy;
 int w;
 int h;
} enemy[5][8];//Define quantos inimigos irão aparecer na tela
struct obj
 nave    = {338, 600, 338, 600, 66, 42},
 tiro   =   {-10, -10, 350, 620, 3, 8},
 tInvad[2] =   {-10, -10, 350, 620, 3, 8};

int main() {
 
 int i, j;
 //Iniciação
 allegro_init();
 install_keyboard();
 set_color_depth(32);
 set_window_title("Space Invaders");//Titulo que será exibido no topo do programa
 set_close_button_callback(sair);//Setar qual botão será utilizado para sair do programa, no caso será utilizado a função sair no lugar de um botão
 set_gfx_mode(GFX_AUTODETECT_WINDOWED, width, height, 0, 0);//Gera a tela em modo grafico nos tamanhos definidos a cima
 
 //Variaveis Locais
 int passo =  0;
 int time  = 65;
 int t    =  0;
 int face  =  0;
 int lft   =  0;
 int dwn   =  0;

//
 tInvad[0].x = height;
 tInvad[1].y = height;
 
 BITMAP *buffer = create_bitmap(width, height);	//Cria uma área do tamanho da tela para depois poder carregar as imagens.bmp 
 BITMAP *background = load_bitmap("img/background.bmp", NULL); //Carrega a imagem de fundo do jogo
 BITMAP *sprites = load_bitmap("img/sprites.bmp", NULL);//Carrega os sprites do objetos

 for(i = 0; i < 5; i++){ //Para cada vez que os inimigos tiverem menos de 5 filas ele irá adicionar mais uma e...
  for(j = 0; j < 10; j++){ //Para cada vez que tiver menos de 10 inimigos por fila ele irá adicionar mais um
   enemy[j][i].x  =  43 + j * 62;
   enemy[j][i].y  = 298 + i * 51;
   enemy[j][i].wx =  43;
   enemy[j][i].wy = 298 + i * 51;
   enemy[j][i].w  =  35;
   enemy[j][i].h  =  24;  
  } //Coordenadas para delimitar os sprites de cada inimigo
 }

 while (!(sai || key[KEY_ESC])) 
 {
  control();
  t++;
  passo = 0;
  if(t > time){
   t = 0;
   passo = 1;
   face = !face;
  }
  
  draw_sprite(buffer, background, 0, 0);//Desenha na tela o plano de fundo
  //Tiro dos Invasores
  if(tInvad[0].y < height) tInvad[0].y += 3;
  if(tInvad[1].y < height) tInvad[1].y += 3;
  
  for(i = 0; i< 5; i++){
   for(j = 0; j< 10; j++){
    if(enemy[j][i].y < height){
     if(enemy[j][i].x <  10 && !lft){ dwn = 1; enemy[j][i].x += passo*15;}
     if(enemy[j][i].x > 704 &&  lft){ dwn = 1; enemy[j][i].x -= passo*15;}
     lft ? (enemy[j][i].x += passo*15) : (enemy[j][i].x -= passo*15);
    }
   /*
    if(enemy[j][i].x == nave.x && tInvad[0].y >= height && enemy[j][i].y < height ){
     tInvad[0].x =  enemy[j][i].x +  enemy[j][i].w/2;
     tInvad[0].x =  enemy[j][i].y +  enemy[j][i].h/2;
     }*/
     if(enemy[9-j][4-i].x == nave.x && tInvad[0].y >= height && enemy[9-j][4-i].y < height ){
     tInvad[0].x =  enemy[9-j][4-i].x +  enemy[9-j][4-i].w/2;
     tInvad[0].x =  enemy[9-j][4-i].y +  enemy[9-j][4-i].h/2;
     }
    masked_blit(sprites, buffer, enemy[j][i].wx + face*62, enemy[j][i].wy, enemy[j][i].x, enemy[j][i].y, enemy[j][i].w, enemy[j][i].h);//Delimita o tamanho dos sprite usado
    
	//colisões
    if(colidir(tiro.x, tiro.y, enemy[j][i].x, enemy[j][i].y, tiro.w, tiro.h, enemy[j][i].w, enemy[j][i].h)){
     enemy[j][i].y = height;
     atirar = 0;
    }
  }
 }
 //Descendo Invasores
  if(dwn){   
   lft = !lft;
   for(i = 0; i< 5; i++){
    for(j = 0; j< 10; j++){
     enemy[j][i].y += 20;
    }
   }
   
  }
  dwn = 0;
  masked_blit(sprites, buffer, nave.wx, nave.wy, nave.x, nave.y, nave.w, nave.h); 
  masked_blit(sprites, buffer, tiro.wx, tiro.wy, tiro.x, tiro.y, tiro.w, tiro.h); 
  masked_blit(sprites, buffer, tInvad[0].wx, tInvad[0].wy, tInvad[0].x, tInvad[0].y, tInvad[0].w, tInvad[0].h);
  masked_blit(sprites, buffer, tInvad[1].wx, tInvad[1].wy, tInvad[1].x, tInvad[1].y, tInvad[1].w, tInvad[1].h);
  draw_sprite(screen, buffer, 0, 0);
  rest(5);
  clear(buffer);
 }
 
 //Finalização lft

 destroy_bitmap(buffer);
 return 0;
}
END_OF_MAIN();

//Função de colisão
int colidir (int Ax, int Ay, int Bx, int By, int Aw, int Ah, int Bw, int Bh){
 if( Ax + Aw> Bx && Ax < Bx + Bw && Ay + Ah > By && Ay < By + Bh)
 return 1;
 return 0;
 }
 
void control(){
 if (key[KEY_LEFT]   && nave.x > 100) nave.x -= 2;//Coloca uma barreira que impede o jogador de ultrapassar a borda do programa e desaparecer a esquerda da tela
 if (key[KEY_RIGHT]  && nave.x < 600) nave.x += 2;//Coloca uma barreira que impede o jogador de ultrapassar a borda do programa e desaparecer a direita da tela
 if (key[KEY_SPACE]  && tiro.y> nave.y && !atirar )   atirar  = 1;//Se for precionado ESPAÇO e o tiro não estiver aparecendo em tela(sprite) a nave irá atirar 
 if (atirar)  tiro.y -= 10;//Se ele estiver atirando a velocidade do tiro na vertical será 10
 if (key[KEY_ENTER]) tiro.y -= 20;//Se ele estiver atirando e precionando ENTER a velocidade do tiro na vertical será aumenta em 20
 if (key[KEY_M])     tiro.y -= 10;//Se ele estiver atirando e precionando M a velocidade do tiro na vertical será aumenta em 10
 if (tiro.y < height/ 4)  atirar  = 0;//Quando o tiro vai muito auto e não atinge nada ele some
 if(!atirar) {
  tiro.x =  nave.x + nave.w / 2;
  tiro.y =  nave.y + nave.h / 2;
 }
 else{}
}
void sair() {sai = 1;}
END_OF_FUNCTION(sair); 
