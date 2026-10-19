#include "Editor/Editor.h"
#include <algorithm>
#include <filesystem>
#include <iomanip>
#include <sstream>

namespace CNashe {
static const SDL_Color fondo{23,25,32,255}, panel{31,34,43,255}, panel2{39,43,54,255},
    borde{62,68,82,255}, textoC{226,230,240,255}, tenue{145,153,171,255},
    acento{80,145,255,255}, verde{72,190,135,255}, amarillo{238,185,75,255};
Editor::Editor(SDL_Renderer* renderer) : r_(renderer) {
    std::filesystem::create_directories("scenes");
    std::filesystem::create_directories("scripts");
    escena_.crearObjeto("Jugador");
    escena_.crearObjeto("Plataforma");
    escena_.objetos[1].y = 220; escena_.objetos[1].ancho = 180; escena_.objetos[1].alto = 32;
    seleccionado_ = 0;
}
void Editor::rect(int x,int y,int w,int h,SDL_Color c) {
    SDL_SetRenderDrawColor(r_,c.r,c.g,c.b,c.a); SDL_Rect a{x,y,w,h}; SDL_RenderFillRect(r_,&a);
}
void Editor::texto(int x,int y,const std::string& s,SDL_Color c,int escala) {
    // Fuente bitmap 5x7 mínima, sin dependencias externas. Letras no reconocidas usan un bloque neutro.
    static const unsigned char glyphs[][7] = {
      {14,17,19,21,25,17,14}, {4,12,4,4,4,4,14}, {14,17,1,2,4,8,31},
      {30,1,1,14,1,1,30}, {2,6,10,18,31,2,2}, {31,16,16,30,1,1,30},
      {6,8,16,30,17,17,14}, {31,1,2,4,8,8,8}, {14,17,17,14,17,17,14},
      {14,17,17,15,1,2,12}, {14,17,17,31,17,17,17}, {30,17,17,30,17,17,30},
      {14,17,16,16,16,17,14}, {30,17,17,17,17,17,30}, {31,16,16,30,16,16,31},
      {31,16,16,30,16,16,16}, {14,17,16,23,17,17,15}, {17,17,17,31,17,17,17},
      {14,4,4,4,4,4,14}, {7,2,2,2,2,18,12}, {17,18,20,24,20,18,17},
      {16,16,16,16,16,16,31}, {17,27,21,21,17,17,17}, {17,25,21,19,17,17,17},
      {14,17,17,17,17,17,14}, {30,17,17,30,16,16,16}, {14,17,17,17,21,18,13},
      {30,17,17,30,20,18,17}, {15,16,16,14,1,1,30}, {31,4,4,4,4,4,4},
      {17,17,17,17,17,17,14}, {17,17,17,17,17,10,4}, {17,17,17,21,21,21,10},
      {17,17,10,4,10,17,17}, {17,17,10,4,4,4,4}, {31,1,2,4,8,16,31}
    };
    auto drawChar = [&](char ch, int px) {
        if (ch == ' ') return;
        if (ch >= 'a' && ch <= 'z') ch = char(ch - 'a' + 'A');
        int idx = -1;
        if (ch >= '0' && ch <= '9') idx = ch - '0';
        else if (ch >= 'A' && ch <= 'Z') idx = 10 + ch - 'A';
        else if (ch == '-') { rect(px, y+3*escala, 4*escala, escala, c); return; }
        else if (ch == '.') { rect(px+2*escala, y+6*escala, escala, escala, c); return; }
        else if (ch == ':') { rect(px+2*escala,y+2*escala,escala,escala,c); rect(px+2*escala,y+5*escala,escala,escala,c); return; }
        else if (ch == '/') { for(int i=0;i<6;i++) rect(px+(4-i%5)*escala,y+i*escala,escala,escala,c); return; }
        else if (ch == '+') { rect(px,y+3*escala,5*escala,escala,c); rect(px+2*escala,y+escala,escala,5*escala,c); return; }
        else { rect(px+escala,y+escala,3*escala,5*escala,c); return; }
        for (int row=0; row<7; ++row)
            for (int col=0; col<5; ++col)
                if (glyphs[idx][row] & (1 << (4-col))) rect(px+col*escala,y+row*escala,escala,escala,c);
    };
    int px=x;
    for(char ch : s) { drawChar(ch,px); px += 6*escala; }
}
void Editor::boton(int x,int y,int w,int h,const std::string& s,bool activo) {
    rect(x,y,w,h,activo?acento:panel2); rect(x,y,w,1,borde);
    texto(x+8,y+8,s,textoC,1);
}
void Editor::campo(int x,int y,const std::string& etiqueta,float valor,bool activo) {
    texto(x,y,etiqueta,tenue);
    rect(x+26,y-4,74,20,activo?SDL_Color{48,65,91,255}:panel2);
    std::ostringstream ss; ss << std::fixed << std::setprecision(0) << valor;
    texto(x+32,y+2,activo?buffer_:ss.str(),textoC);
}
void Editor::guardar() {
    if (escena_.guardar("scenes/escena.cns")) estado_="Escena guardada";
    else estado_="Error al guardar";
}
void Editor::cargar() {
    if (escena_.cargar("scenes/escena.cns")) {
        seleccionado_ = escena_.objetos.empty() ? -1 : 0; estado_="Escena cargada";
    } else estado_="No se pudo cargar scenes/escena.cns";
}
void Editor::procesarEvento(const SDL_Event& e) {
    if (e.type == SDL_QUIT) salir=true;
    if (e.type == SDL_KEYDOWN && campoActivo_) {
        if (e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_KP_ENTER) {
            try {
                float v=std::stof(buffer_);
                if(seleccionado_>=0 && seleccionado_<(int)escena_.objetos.size()) {
                    if(campoActivo_==1) escena_.objetos[seleccionado_].x=v;
                    if(campoActivo_==2) escena_.objetos[seleccionado_].y=v;
                }
            } catch(...) {}
            campoActivo_=0; buffer_.clear(); estado_="Coordenadas actualizadas";
        } else if(e.key.keysym.sym==SDLK_ESCAPE) { campoActivo_=0; buffer_.clear(); }
        else if(e.key.keysym.sym==SDLK_BACKSPACE) { if(!buffer_.empty()) buffer_.pop_back(); }
        else {
            SDL_Keycode k=e.key.keysym.sym;
            if((k>=SDLK_0&&k<=SDLK_9)||k==SDLK_MINUS||k==SDLK_PERIOD||k==SDLK_KP_0||k==SDLK_KP_1||k==SDLK_KP_2||k==SDLK_KP_3||k==SDLK_KP_4||k==SDLK_KP_5||k==SDLK_KP_6||k==SDLK_KP_7||k==SDLK_KP_8||k==SDLK_KP_9) {
                if(k>=SDLK_KP_0&&k<=SDLK_KP_9) buffer_+=char('0'+(k-SDLK_KP_0));
                else if(k==SDLK_PERIOD) buffer_+='.';
                else if(k==SDLK_MINUS) buffer_+='-';
                else buffer_+=char('0'+(k-SDLK_0));
            }
        }
        return;
    }
    if(e.type != SDL_MOUSEBUTTONDOWN || e.button.button != SDL_BUTTON_LEFT) return;
    int x=e.button.x, y=e.button.y;
    if(y<48) {
        if(x>=10&&x<100) { runtime_.detener(); estado_="Runtime detenido"; }
        else if(x>=105&&x<195) { escena_.crearObjeto("Objeto"+std::to_string(escena_.objetos.size()+1)); seleccionado_=(int)escena_.objetos.size()-1; }
        else if(x>=200&&x<290) guardar();
        else if(x>=295&&x<385) cargar();
        else if(x>=390&&x<475) { modo3D_=!modo3D_; estado_=modo3D_?"Vista 3D esquematica":"Vista 2D"; }
        else if(x>=480&&x<590) { std::string out; runtime_.ejecutarArchivo("scripts/demo.cn",escena_,out); estado_="Script .cn iniciado (ver scripts/demo.cn)"; }
        return;
    }
    if(x>=890 && y>=145 && y<190) { campoActivo_=1; buffer_.clear(); return; }
    if(x>=890 && y>=195 && y<240) { campoActivo_=2; buffer_.clear(); return; }
    if(x>=880 && y>=70 && y<125) { estado_="Selecciona X o Y para editar"; return; }
    if(x>=205&&x<870&&y>=70&&y<660) {
        float lx=float(x-205), ly=float(y-70);
        for(int i=(int)escena_.objetos.size()-1;i>=0;--i) {
            auto& o=escena_.objetos[i];
            if(o.visible && lx>=o.x&&lx<=o.x+o.ancho&&ly>=o.y&&ly<=o.y+o.alto) { seleccionado_=i; return; }
        }
        seleccionado_=-1;
    }
    if(x<195 && y>=80) {
        int idx=(y-80)/30;
        if(idx>=0&&idx<(int)escena_.objetos.size()) seleccionado_=idx;
    }
}
void Editor::dibujar() {
    rect(0,0,1200,720,fondo);
    rect(0,0,1200,48,panel);
    boton(10,8,86,32,"DETENER");
    boton(105,8,86,32,"+ OBJETO");
    boton(200,8,86,32,"GUARDAR");
    boton(295,8,86,32,"CARGAR");
    boton(390,8,82,32,modo3D_?"VISTA 3D":"VISTA 2D",modo3D_);
    boton(480,8,106,32,"EJECUTAR",runtime_.activo());
    texto(610,18,"CNASHE STUDIO 0.2",textoC);
    rect(0,48,195,622,panel); texto(14,62,"OBJETOS",textoC);
    for(size_t i=0;i<escena_.objetos.size()&&i<18;i++) {
        if((int)i==seleccionado_) rect(8,78+(int)i*30,178,25,SDL_Color{48,65,91,255});
        texto(16,86+(int)i*30,escena_.objetos[i].nombre, (int)i==seleccionado_?textoC:tenue);
    }
    rect(195,48,680,622,SDL_Color{19,21,27,255});
    // Grilla del viewport
    for(int gx=205;gx<870;gx+=25) { SDL_SetRenderDrawColor(r_,37,40,48,255); SDL_RenderDrawLine(r_,gx,70,gx,660); }
    for(int gy=70;gy<660;gy+=25) { SDL_SetRenderDrawColor(r_,37,40,48,255); SDL_RenderDrawLine(r_,205,gy,870,gy); }
    texto(210,54,modo3D_?"VISTA 3D ESQUEMATICA":"VISTA 2D",tenue);
    for(size_t i=0;i<escena_.objetos.size();++i) {
        const auto& o=escena_.objetos[i]; if(!o.visible) continue;
        SDL_Color c{o.r,o.g,o.b,255};
        int px=205+(int)o.x, py=70+(int)o.y;
        if(modo3D_) { // Proyección isométrica ilustrativa, no renderizador 3D real.
            px += (int)(o.z*0.5f); py -= (int)(o.z*0.25f);
            rect(px+8,py-8,(int)o.ancho,(int)o.alto,SDL_Color{50,65,82,255});
            rect(px,py,(int)o.ancho,(int)o.alto,c);
            SDL_SetRenderDrawColor(r_,c.r,c.g,c.b,255);
            SDL_RenderDrawLine(r_,px,py,px+8,py-8);
            SDL_RenderDrawLine(r_,px+(int)o.ancho,py,px+(int)o.ancho+8,py-8);
        } else rect(px,py,(int)o.ancho,(int)o.alto,c);
        if((int)i==seleccionado_) {
            SDL_SetRenderDrawColor(r_,255,255,255,255);
            SDL_Rect outline{px-2,py-2,(int)o.ancho+4,(int)o.alto+4}; SDL_RenderDrawRect(r_,&outline);
        }
    }
    rect(875,48,325,622,panel); texto(895,62,"INSPECTOR",textoC);
    if(seleccionado_>=0&&seleccionado_<(int)escena_.objetos.size()) {
        auto& o=escena_.objetos[seleccionado_];
        texto(895,94,o.nombre,textoC);
        texto(895,120,"TRANSFORMACION",tenue);
        campo(895,155,"X",o.x,campoActivo_==1);
        campo(895,205,"Y",o.y,campoActivo_==2);
        texto(895,255,"ANCHO",tenue); texto(970,255,std::to_string((int)o.ancho),textoC);
        texto(895,280,"ALTO",tenue); texto(970,280,std::to_string((int)o.alto),textoC);
        texto(895,325,"3D: Z / ROTACION / ESCALA",tenue);
        texto(895,350,"Z "+std::to_string((int)o.z),textoC);
        texto(895,375,"ROT "+std::to_string((int)o.rotZ),textoC);
        texto(895,400,"ESCALA "+std::to_string((int)o.scaleX),textoC);
        texto(895,440,"SPRITE",tenue);
        texto(895,462,o.spritePath.empty()?"SIN SPRITE":"SPRITE ASIGNADO",textoC);
        texto(895,500,"COLISION AABB",tenue);
        texto(895,522,"ACTIVA EN RUNTIME",textoC);
        texto(895,570,"EDITA X/Y: CLIC + NUMEROS",tenue);
        texto(895,590,"ENTER CONFIRMA",tenue);
    } else texto(895,100,"Selecciona un objeto",tenue);
    rect(0,670,1200,50,panel); texto(14,688,"ESTADO: "+estado_,estado_=="Escena guardada"?verde:tenue);
    texto(780,688,"CNASHE / PROTOTIPO",tenue);
    SDL_RenderPresent(r_);
}
}
