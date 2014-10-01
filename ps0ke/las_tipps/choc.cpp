/*
  This code is originally from a 1K .exe demo called 'Chocolux' by Auld.
    http://sizecoding.blogspot.com/2008/02/chocolux-1k-intro.html
    http://www.pouet.net/prod.php?which=49796
  I 'ported' it to compile/run with MinGW.
    g++ -Wall -s -O2 -mwindows -o choc.exe choc.cpp -lopengl32 -lglu32
  To see it running inside WebGL:
    http://www.satine.org/research/webkit/webgl/chocolux.html

Bugfixes/Changes:
  1. Fixed warnings in DEVMODE about unions not having proper amount of braces { }
  2. Added message loop to prevent Win7 from showing busy cursor
  3. Cleaned up the heinous wglGetProcAddress casts
     e.g.
        p = ((PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram"))();
  4. Indented the shader source so they are a (little) more readable now
  Compare to the original code:
    http://scener.neostrada.pl/releases/chocolux_code.zip
*/
#include <windows.h>
#include <GL/gl.h>
#include "glext.h" // http://www.opengl.org/registry/api/glext.h

int SCREENX = 1024;
int SCREENY =  768;

DEVMODE dm =
{
     "", 0, 0, sizeof(dm), 0, DM_PELSWIDTH|DM_PELSHEIGHT, {{0,0,0,0,0,0,0,0}}, 0,0,0,0,0,
     "", 0, 0, SCREENX, SCREENY, {0}, 0,0,0,0,0,0,0,0,0
};

PIXELFORMATDESCRIPTOR pfd =
{
    sizeof(pfd), 1, PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
    32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32, 0, 0, 0, 0, 0, 0, 0
};

const GLchar *vsh = "\
varying vec3 s[4];\
void main(){\
    gl_Position=gl_Vertex;\
    s[0]=vec3(0);\
    s[3]=vec3(sin(abs(gl_Vertex.x*.0001)),cos(abs(gl_Vertex.x*.0001)),0);\
    s[1]=s[3].zxy;\
    s[2]=s[3].zzx;\
}";

const GLchar *fsh = "\
varying vec3 s[4];\
void main(){\
    float t,b,c,h=0;\
    vec3 m,n,p=vec3(.2),d=normalize(.001*gl_FragCoord.rgb-p);\
    for(int i=0;i<4;i++){\
        t=2;\
        for(int i=0;i<4;i++){\
            b=dot(d,n=s[i]-p);\
            c=b*b+.2-dot(n,n);\
            if(b-c<t)if(c>0){m=s[i];t=b-c;}\
        }\
        p+=t*d;\
        d=reflect(d,n=normalize(p-m));\
        h+=pow(n.x*n.x,44.)+n.x*n.x*.2;\
        }\
    gl_FragColor=vec4(h,h*h,h*h*h*h,h);\
}";

void compileShader()
{
    PFNGLATTACHSHADERPROC  glAttachShader  = (PFNGLATTACHSHADERPROC ) wglGetProcAddress("glAttachShader" );
    PFNGLCOMPILESHADERPROC glCompileShader = (PFNGLCOMPILESHADERPROC) wglGetProcAddress("glCompileShader");
    PFNGLCREATEPROGRAMPROC glCreateProgram = (PFNGLCREATEPROGRAMPROC) wglGetProcAddress("glCreateProgram");
    PFNGLCREATESHADERPROC  glCreateShader  = (PFNGLCREATESHADERPROC ) wglGetProcAddress("glCreateShader" );
    PFNGLLINKPROGRAMPROC   glLinkProgram   = (PFNGLLINKPROGRAMPROC  ) wglGetProcAddress("glLinkProgram"  );
    PFNGLUSEPROGRAMPROC    glUseProgram    = (PFNGLUSEPROGRAMPROC   ) wglGetProcAddress("glUseProgram"   );
    PFNGLSHADERSOURCEPROC  glShaderSource  = (PFNGLSHADERSOURCEPROC ) wglGetProcAddress("glShaderSource" );

    GLuint s,p;
    p = glCreateProgram();

    s = glCreateShader( GL_VERTEX_SHADER );
    glShaderSource( s, 1, &vsh, NULL );
    glCompileShader( s );
    glAttachShader( p, s );

    s = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource( s, 1, &fsh, NULL );
    glCompileShader( s );
    glAttachShader( p, s );

    glLinkProgram( p );
    glUseProgram( p );
}

//int main( int nArg, char * aArg[] ); // also works with MinGW
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
    ChangeDisplaySettings( &dm, CDS_FULLSCREEN );

    HDC hDC = GetDC( CreateWindow( "edit", 0, WS_POPUP|WS_VISIBLE|WS_MAXIMIZE, 0, 0, 0, 0, 0, 0, 0, 0 ) );
    SetPixelFormat ( hDC, ChoosePixelFormat( hDC, &pfd ), &pfd );
    wglMakeCurrent ( hDC, wglCreateContext( hDC ) );
    ShowCursor( FALSE );
    compileShader();

    while( !GetAsyncKeyState( VK_ESCAPE ) )
    {
        MSG msg;
        while (PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ))
        {
            //TranslateMessage( &msg ); // Don't have any keys that need translating to WM_CHAR messages
            DispatchMessage( &msg );
        }
        int t = GetTickCount();
        glRecti( t, t, -t, -t );
        SwapBuffers( hDC );
    }
    ExitProcess( 0 );
    return 0;
}

