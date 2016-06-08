#ifndef _EZGLSL_H_
#define _EZGLSL_H_

#include <GL/glew.h>
#include <GL/gl.h>
      

#include <vector>
#include <iostream>

class ezShaderProgram {
    friend class ezShaderObject;

public:
    ezShaderProgram();
    ~ezShaderProgram();
    
    int load(char* filename);   //!< read file, if result is 0 everything is ok. -1: File not found, -2: Empty File, -3: no memory
    void loadFromMemory(const char* program); //!< load program from char array, make sure program is 0 terminated! 
    
    
    bool compile(void);         //!< compile program

    char* getCompilerLog(void);  //!< get compiler messages
 
protected:

    int                 program_type;          //!< 1=Vertex Program, 2=Fragment Program, 0=none

    GLhandleARB         ProgramObject;         //!< Program Object
    GLubyte*            ShaderSource;          //!< ASCII Source-Code
    
    GLcharARB*          compiler_log;
    
    bool                is_compiled;            //!< true if compiled
    bool                _memalloc;               //!< true if shader allocated memory

    
};

class ezVertexShader : public ezShaderProgram {
  public:
       ezVertexShader();
       ~ezVertexShader(); 
};

class ezFragmentShader : public ezShaderProgram {
 public:
    ezFragmentShader();
    ~ezFragmentShader();
 
};

class ezShaderObject {
public:
    ezShaderObject();            // Standard Constructor
    ~ezShaderObject();           // Destructor

    void addShader(ezShaderProgram* ShaderProgram); //!< add a Vertex or Fragment Program
    
    bool link(void);            //!< Link all Shaders
    char* getLinkerLog(void);   //!< get Linker messages

    void begin();	//!< use Shader. OpenGL calls will go through shader.
    void end();		//!< Stop using this shader. OpenGL calls will go through regular pipeline.
    
    bool oglslEnabled(void);    //!< returns true if OGLSL is enabled. It is possible user hardware doesn't support OGLSL!

    // Send Variables to Program
 
    bool sendUniform1f(char* varname, GLfloat v0); //!< send float to program
    bool sendUniform2f(char* varname, GLfloat v0, GLfloat v1); //!< send vec2 to program
    bool sendUniform3f(char* varname, GLfloat v0, GLfloat v1, GLfloat v2); //!< send vec3 to program
    bool sendUniform4f(char* varname, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3); //!< send vec4 to program

    bool sendUniform1i(char* varname, GLint v0);
    bool sendUniform2i(char* varname, GLint v0, GLint v1);
    bool sendUniform3i(char* varname, GLint v0, GLint v1, GLint v2);
    bool sendUniform4i(char* varname, GLint v0, GLint v1, GLint v2, GLint v3);

    bool sendUniform1fv(char* varname, GLsizei count, GLfloat *value);
    bool sendUniform2fv(char* varname, GLsizei count, GLfloat *value);
    bool sendUniform3fv(char* varname, GLsizei count, GLfloat *value);
    bool sendUniform4fv(char* varname, GLsizei count, GLfloat *value);

    bool sendUniform1iv(char* varname, GLsizei count, GLint *value);
    bool sendUniform2iv(char* varname, GLsizei count, GLint *value);
    bool sendUniform3iv(char* varname, GLsizei count, GLint *value);
    bool sendUniform4iv(char* varname, GLsizei count, GLint *value);

    bool sendUniformMatrix2fv(char* varname, GLsizei count, GLboolean transpose, GLfloat *value);
    bool sendUniformMatrix3fv(char* varname, GLsizei count, GLboolean transpose, GLfloat *value);
    bool sendUniformMatrix4fv(char* varname, GLsizei count, GLboolean transpose, GLfloat *value);


    // Receive Uniform variables:
    void GetUniformfv(char* name, GLfloat* values);
    void GetUniformiv(char* name, GLint* values); 


    void manageMemory(void){_mM = true;}

    // Turn off all Shaders:
    static void useShader(bool b)		//!< Deactivate Shader
	{ 
		_noshader = b; 
	}  
       
private:
    GLint GetUniLoc(const GLcharARB *name);      // get location of a variable
    
    GLhandleARB         ShaderObject;            // Shader Object
    

    GLcharARB*          linker_log;
    bool                is_linked;
    std::vector<ezShaderProgram*> ShaderList;     // List of all Shader Programs

    bool                _mM;
    static bool         _noshader;
      
};

//-----------------------------------------------------------------------------
// To simplify the process loading/compiling/linking shaders I created this
// high level interface to setup a vertex/fragment shader.
class ezShaderManager {
public:
    ezShaderManager();
    ~ezShaderManager();

    ezShaderObject* loadfromFile(char* vertexFile, char* fragmentFile);    // load vertex/fragment shader from file
    ezShaderObject* loadfromMemory(const char* vertexMem, const char* fragmentMem);
        
    bool           free(ezShaderObject* o);

private:
    std::vector<ezShaderObject*>  _shaderObjectList;      
};


#endif // _EZGLSL_H_
