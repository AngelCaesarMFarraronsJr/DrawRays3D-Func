#include <GL/glut.h>
#include <math.h>

#define PI 3.1415926535
#define P2 PI/2
#define P3 3*PI/2
#define DR 0.0174533

// Global variables (adjust these to match your program)
int mapX=8, mapY=8, mapS=64;
int map[] = {
    1,1,1,1,1,1,1,1,
    1,0,0,0,0,0,0,1,
    1,0,1,0,0,1,0,1,
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,0,1,0,0,1,0,1,
    1,0,0,0,0,0,0,1,
    1,1,1,1,1,1,1,1,
};
float px=300, py=300;
float pa=0;

float dist(float ax, float ay, float bx, float by, float ang)
{
    return sqrt((bx-ax)*(bx-ax)+(by-ay)*(by-ay));
}

void drawRays3D()
{
    int r,mx,my,mp,dof; float rx,ry,ra,xo,yo,disT;
    ra=pa-DR*30; if(ra<0){ ra+=2*PI;} if(ra>2*PI){ ra-=2*PI;}
    for(r=0;r<60;r++)
    {
        //---Check Horizontal Lines---
        dof=0;
        float disH=1000000,hx,hy;
        float aTan=-1/tan(ra);
        if(ra>PI){ ry=(((int)py>>6)<<6)-0.0001; rx=(py-ry)*aTan+px; yo=-64; xo=-yo*aTan;}//looking up
        if(ra<PI){ ry=(((int)py>>6)<<6)+64;     rx=(py-ry)*aTan+px; yo= 64; xo=-yo*aTan;}//looking down
        if(ra==0 || ra==PI){ rx=px; ry=py; dof=8;}//looking straight left or right
        while(dof<8)
        {
            mx=(int)(rx)>>6; my=(int)(ry)>>6; mp=my*mapX+mx;
            if(mp>0 && mp<mapX*mapY && map[mp]==1){ hx=rx; hy=ry; disH=dist(px,py,hx,hy,ra); dof=8;}//hit wall
            else{ rx+=xo; ry+=yo; dof+=1;}//next line
        }
        
        //---Check Vertical Lines---
        dof=0;
        float disV=1000000,vx=px,vy=py;
        float nTan=-tan(ra);
        if(ra>P2 && ra<P3){ rx=(((int)px>>6)<<6)-0.0001; ry=(px-rx)*nTan+py; xo=-64; yo=-xo*nTan;}//looking left
        if(ra<P2 || ra>P3){ rx=(((int)px>>6)<<6)+64;     ry=(px-rx)*nTan+py; xo= 64; yo=-xo*nTan;}//looking right
        if(ra==0 || ra==PI){ rx=px; ry=py; dof=8;}//looking straight up or down
        while(dof<8)
        {
            mx=(int)(rx)>>6; my=(int)(ry)>>6; mp=my*mapX+mx;
            if(mp>0 && mp<mapX*mapY && map[mp]==1){ vx=rx; vy=ry; disV=dist(px,py,vx,vy,ra); dof=8;}//hit wall
            else{ rx+=xo; ry+=yo; dof+=1;}//next line
        }
        
        if(disV<disH){ rx=vx; ry=vy; disT=disV; glColor3f(0.4,0.8,0.4);}                //vertical    wall hit (light green)
        if(disH<disV){ rx=hx; ry=hy; disT=disH; glColor3f(0.2,0.6,0.2);}                //horizontal wall hit (dark green)
        glLineWidth(3); glBegin(GL_LINES); glVertex2i(px,py); glVertex2i(rx,ry); glEnd();
        //---Draw 3D Walls---
        float ca=pa-ra; if(ca<0){ ca+=2*PI;} if(ca>2*PI){ ca-=2*PI;} disT=disT*cos(ca);//fix fisheye
        float lineH=(mapS*320)/disT; if(lineH>320){lineH=320;}                     //line height
        float lineO=160-lineH/2;                                                   //line offset
        
        //---Draw Ceiling (light blue)---
        glColor3f(0.53, 0.81, 0.92);
        glLineWidth(8);
        glBegin(GL_LINES);
        glVertex2i(r*8+530, 0);
        glVertex2i(r*8+530, lineO);
        glEnd();
        
        //---Draw Wall---
        if(disV<disH){ glColor3f(0.4,0.8,0.4); }  //vertical wall (light green)
        if(disH<disV){ glColor3f(0.2,0.6,0.2); }  //horizontal wall (dark green)
        glLineWidth(8);
        glBegin(GL_LINES);
        glVertex2i(r*8+530, lineO);
        glVertex2i(r*8+530, lineH+lineO);
        glEnd();
        
        //---Draw Floor (grey)---
        glColor3f(0.5, 0.5, 0.5);
        glLineWidth(8);
        glBegin(GL_LINES);
        glVertex2i(r*8+530, lineH+lineO);
        glVertex2i(r*8+530, 320);
        glEnd();
        
        ra+=DR; if(ra<0){ ra+=2*PI;} if(ra>2*PI){ ra-=2*PI;}
    }
}
