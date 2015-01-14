#!/usr/bin/python2.6  
# -*- coding: utf-8 -*-  
#encoding=utf-8

import sys
import platform
import os
'''
Created on 2014年11月11日
@author: kerry
@contact: kerry@miglab.com
@version: 0.0.1
@summary: 用于plugins 的基础框架的代码生成
'''

#init        h文件生成
#init        cc文件生成
#manager     h文件生成
#manager     cc文件生成

#param: path  project  name 

FILE_NAME_HEAD_INIT = ""
FILE_NAME_CPP_INIT = ""
FILE_NAME_HEAD_LOGIC = ""
FILE_NAME_CPP_LOGIC = ""

def write_file(path,data):
    print path
    file_object = open(path,'w')
    file_object.writelines(data)
    file_object.close()
        
def generation_macro_definition(worksapce,project,file,other):
    return "__"+worksapce.upper()+"__"+project.upper()+"__"+file.upper()+"_"+other.upper()+"___"

#框架头文件
def generation_init_head(worksapce,project,file,name,path):
    #宏定义
    #print generation_macro_definition(worksapce,project,file,"init")
    global FILE_NAME_HEAD_INIT
    content =  "#ifndef\x20" + generation_macro_definition(worksapce,project,file,"init")+"\n#define\x20"+ \
        generation_macro_definition(worksapce,project,file,"init")+"\n#ifdef __cplusplus\n"+ \
        "extern \"C\" {\n"+"#endif\nint "+name+"_plugin_init(struct plugin *pl);\n"+ \
        "#ifdef __cplusplus\n}\n#endif\n#endif\n\n"
    #写入文件
    FILE_NAME_HEAD_INIT = name+"_init.h"
    write_file(path+FILE_NAME_HEAD_INIT,content)
    
#逻辑头文件
def generation_logic_head(worksapce,project,file,name,path):
    global FILE_NAME_HEAD_LOGIC
    content = "#ifndef\x20" + generation_macro_definition(worksapce,project,file,"logic")+"\n#define\x20"+ \
        generation_macro_definition(worksapce,project,file,"logic")+"\n#include\x20\"common.h\"\n\n\nnamespace\x20"+project+"_logic{\n\n" + \
        "class "+(name+"logic").title()+"{\n\npublic:\n\x20\x20\x20\x20"+(name+"logic").title()+"();\n"+ \
        "\x20\x20\x20\x20virtual\x20~"+(name+"logic").title()+"();\n\nprivate:\n\x20\x20\x20\x20static\x20"+ \
        (name+"logic").title()+"\x20\x20\x20\x20" + "*instance_;\n\n"+"public:\n\x20\x20\x20\x20static\x20" + \
        (name+"logic").title()+"\x20"+"*GetInstance();"+"\n\x20\x20\x20\x20static\x20"+"void\x20" +"FreeInstance();\n\n" + \
        "public:\n" + "\x20\x20\x20\x20bool On"+name.title()+"Connect\x20(struct\x20server\x20*srv,const\x20int\x20socket);\n\n"+\
        "\x20\x20\x20\x20bool\x20On"+name.title()+"Message\x20(struct\x20server *srv, const\x20int\x20socket,const\x20void\x20*msg,"+\
        "\x20const\x20int\x20len);\n\n"+"\x20\x20\x20\x20bool\x20On"+name.title()+"Close\x20(struct server\x20*srv,const\x20int\x20socket);"+\
        "\n\n\x20\x20\x20\x20bool\x20OnBroadcastConnect(struct\x20server\x20*srv,const\x20int\x20socket,const\x20void\x20*data,\x20const\x20int\x20len);\n\n"+\
        "\x20\x20\x20\x20bool\x20OnBroadcastMessage\x20(struct\x20server\x20*srv,\x20const\x20int\x20socket,\x20const\x20void\x20*msg,\x20const\x20int\x20len);\n\n"+\
        "\x20\x20\x20\x20bool\x20OnBroadcastClose\x20(struct\x20server\x20*srv,\x20const\x20int\x20socket);\n\n"+\
        "\x20\x20\x20\x20bool\x20OnIniTimer\x20(struct\x20server\x20*srv);\n\n\x20\x20\x20\x20bool\x20OnTimeout\x20"+\
        "(struct\x20server\x20*srv,\x20char*\x20id,\x20int\x20opcode,\x20int\x20time);\n"+\
        "\n\nprivate:\n\n\x20\x20\x20\x20bool\x20Init();\n};\n\n\n}\n\n#endif\n\n"

    #写入文件
    FILE_NAME_HEAD_LOGIC = name+"_logic.h"
    write_file(path+FILE_NAME_HEAD_LOGIC,content)
    
    
#框架下实现文件
def generation_init_cpp_start(project,name):
    content ="static\x20void\x20*On"+name.title()+"Start(){\n\n\x20\x20\x20\x20signal(SIGPIPE,SIG_IGN);\n\x20\x20\x20\x20struct\x20"+ \
        (name+"plugin").lower()+"*\x20"+name.lower()+"=(struct\x20"+ (name+"plugin").lower()+"*)" +"calloc(1,sizeof("+\
        "struct\x20"+ (name+"plugin").lower()+"));\n\n\x20\x20\x20\x20"+name.lower()+"->id=\"\";\n\n\x20\x20\x20\x20"+name.lower()+ \
        "->name=\"\";\n\n\x20\x20\x20\x20"+name.lower()+ "->version=\"\";\n\n\x20\x20\x20\x20"+name.lower()+"->provider=\"\";\n\n" + \
        "\x20\x20\x20\x20if(!"+project+"_logic::"+(name+"logic").title()+"::GetInstance())\n\x20\x20\x20\x20\x20\x20\x20\x20assert(0);" + \
        "\n\n\x20\x20\x20\x20return\x20"+name.lower()+"\x20;\n\n}"
        
    return content

def generation_init_cpp_shutdown(project,name):
    content = "static\x20handler_t\x20On"+name.title()+"Shutdown(struct\x20server*\x20srv,void*\x20pd){\n\n" + \
        "\x20\x20\x20\x20"+project+"_logic::"+(name+"logic").title()+"::FreeInstance();\n\n\x20\x20\x20\x20" + \
        "return\x20HANDLER_GO_ON;\n}"
    
    return content

def generation_init_cpp_connect(project,name):
    content = "static\x20handler_t\x20On"+name.title()+"Connect(struct server *srv, int fd, void *data, int len){\n\n" + \
        "\x20\x20\x20\x20"+project+"_logic::"+(name+"logic").title()+"::GetInstance()->On"+name.title()+"Connect("+ \
        "srv,fd);"+"\n\n\x20\x20\x20\x20return\x20HANDLER_GO_ON;\n}"
    
    return content

def generation_init_cpp_message(project,name):
    content = "static\x20handler_t\x20On"+name.title()+"Message(struct\x20server\x20*srv,\x20int\x20fd,\x20void\x20*data,\x20int\x20len){\n\n" + \
        "\x20\x20\x20\x20"+project+"_logic::"+(name+"logic").title()+"::GetInstance()->On"+name.title()+"Message("+ \
        "srv,fd,data,len);"+"\n\n\x20\x20\x20\x20return\x20HANDLER_GO_ON;\n}"
    
    return content

def generation_init_cpp_close(project,name):
    content = "static\x20handler_t\x20On"+name.title()+"Close(struct\x20server\x20*srv,\x20int\x20fd){\n\n" + \
        "\x20\x20\x20\x20"+project+"_logic::"+(name+"logic").title()+"::GetInstance()->On"+name.title()+"Close("+ \
        "srv,fd);"+"\n\n\x20\x20\x20\x20return\x20HANDLER_GO_ON;\n}"
    
    return content

def generation_init_unknow(project,name):
    content = "static\x20handler_t\x20On"+"Unknow(struct\x20server\x20*srv,\x20int fd,\x20void *data,\x20int\x20len){\n\n" + \
        "\n\x20\x20\x20\x20return\x20"+"HANDLER_GO_ON;\n\n}"
    return content

def generation_init_bro_connect(project,name):
    content = "static\x20handler_t\x20OnBroadcastConnect(struct\x20server*\x20srv,\x20int\x20fd,\x20void\x20*data," +\
        "\x20int\x20len){\n\n\n\x20\x20\x20\x20"+project+"_logic::"+(name+"logic").title()+"::GetInstance()->OnBroadcastConnect("+\
        "srv,fd,data,len);\n\n\x20\x20\x20\x20return\x20HANDLER_GO_ON;\n\n}"
    return content

def generation_init_bro_close(project,name):
    content = "static\x20handler_t\x20OnBroadcastClose(struct\x20server*\x20srv,\x20int\x20fd){" +\
       " \n\n\n\x20\x20\x20\x20"+project+"_logic::"+(name+"logic").title()+"::GetInstance()->OnBroadcastClose(srv,fd);\n\n" + \
       "\x20\x20\x20\x20return\x20HANDLER_GO_ON;\n\n\n}"
    return content
    
def generation_init_bro_message(project,name):
    content = "static\x20handler_t\x20OnBroadcastMessage(struct\x20server*\x20srv,\x20int\x20fd,\x20void\x20*data," +\
        "\x20int\x20len){""\n\n\n\x20\x20\x20\x20"+project+"_logic::"+(name+"logic").title()+"::GetInstance()->OnBroadcastMessage(srv,fd,data,len);\n\n" + \
        "\x20\x20\x20\x20return\x20HANDLER_GO_ON;\n\n\n}"
    return content
        
def generation_init_timer(project,name):
    content = "static\x20handler_t\x20OnIniTimer(struct\x20server*\x20srv){" +\
       " \n\n\n\x20\x20\x20\x20"+project+"_logic::"+(name+"logic").title()+"::GetInstance()->OnIniTimer(srv);\n\n" + \
       "\x20\x20\x20\x20return\x20HANDLER_GO_ON;\n\n\n}"
    return content
    
def generation_init_time_out(project,name):
    content = "static\x20handler_t\x20OnTimeOut(struct\x20server*\x20srv,char*\x20id,\x20int\x20opcode,\x20int\x20time){" +\
       " \n\n\n\x20\x20\x20\x20"+project+"_logic::"+(name+"logic").title()+"::GetInstance()->OnTimeout(srv,id,opcode,time);\n\n" + \
       "\x20\x20\x20\x20return\x20HANDLER_GO_ON;\n\n\n}"
    return content
       
def generation_init_plugin(project,name):
    content = "int\x20"+name+"_plugin_init(struct plugin *pl){\n\n"+"\n\x20\x20\x20\x20pl->init=On"+name.title()+"Start;\n\n"+ \
        "\x20\x20\x20\x20pl->clean_up\x20=\x20On"+name.title()+"Shutdown;\n\n"+ \
        "\x20\x20\x20\x20pl->connection\x20=\x20On"+name.title()+"Connect;\n\n"+\
        "\x20\x20\x20\x20pl->connection_close\x20=\x20On"+name.title()+"Close;\n\n" +\
        "\x20\x20\x20\x20pl->connection_close_srv\x20=\x20OnBroadcastClose;\n\n"+ \
        "\x20\x20\x20\x20pl->connection_srv\x20=\x20OnBroadcastConnect;\n\n" +\
        "\x20\x20\x20\x20pl->handler_init_time\x20=\x20OnIniTimer;\n\n" +\
        "\x20\x20\x20\x20pl->handler_read\x20=\x20On"+name.title()+"Message;\n\n" +\
        "\x20\x20\x20\x20pl->handler_read_srv\x20=\x20OnBroadcastMessage;\n\n" +\
        "\x20\x20\x20\x20pl->handler_read_other\x20=\x20OnUnknow;\n\n"+\
        "\x20\x20\x20\x20pl->time_msg\x20=\x20OnTimeOut;\n\n"+\
        "\x20\x20\x20\x20pl->data\x20=\x20NULL;\n\n" +\
        "\x20\x20\x20\x20return 0;\n\n}"
    return content
    
    
def generation_init_cpp(worksapce,project,file,name,path):
    global FILE_NAME_HEAD_LOGIC
    content = "#include\x20\""+FILE_NAME_HEAD_INIT+"\"\n#include\x20\""+FILE_NAME_HEAD_LOGIC+"\"\n#include\x20\"common.h\"\n" +\
        "#include\x20\"plugins.h\"\n\n\n"+"struct\x20"+(name+"plugin").lower()+"{\n\x20\x20\x20\x20char*\x20id;\n\x20\x20\x20\x20" + \
        "char*\x20name;\n\x20\x20\x20\x20char*\x20version;\n\x20\x20\x20\x20char*\x20provider;\n};\n\n\n" + \
        generation_init_cpp_start(project,name)+"\n\n"+generation_init_cpp_shutdown(project,name)+"\n\n" + \
        generation_init_cpp_connect(project,name)+"\n\n"+generation_init_cpp_message(project,name)+"\n\n"+ \
        generation_init_cpp_close(project,name)+"\n\n" + generation_init_unknow(project,name)+"\n\n" + \
        generation_init_bro_connect(project,name)+"\n\n"+ generation_init_bro_close(project,name)+"\n\n"+ \
        generation_init_bro_message(project,name)+"\n\n" + generation_init_timer(project,name)+"\n\n" + \
        generation_init_time_out(project,name)+"\n\n" +"\n\n" + "\n\n\n"+generation_init_plugin(project,name)+"\n\n"
        
                
       #写入文件
    FILE_NAME_CPP_INIT = name+"_init.cc"
    write_file(path+FILE_NAME_CPP_INIT,content)

#逻辑下实现文件
def generation_logic_construction(project,name):
    content = (name+"logic").title()+"::" + \
        (name+"logic").title()+"(){\n\x20\x20\x20if(!Init())\n\x20\x20\x20\x20\x20\x20assert(0);\n}"
    return content

def generation_logic_destruct(project,name):
    content = (name+"logic").title()+"::~" + \
        (name+"logic").title()+"(){\n}"
        
    return content

def generation_logic_init(project,name):
    content = "bool\x20"+(name+"logic").title()+"::Init(){\n\x20\x20\x20\x20return\x20true;\n}"
    return content

def generation_logic_getinstance(project,name):
    content = "\n\n"+(name+"logic").title()+"*\n"+(name+"logic").title()+"::GetInstance(){\n\n\x20\x20\x20\x20" + \
        "if(instance_==NULL)\n\x20\x20\x20\x20\x20\x20\x20\x20" + \
        "instance_\x20=\x20new\x20"+(name+"logic").title()+"();\n\n\x20\x20\x20\x20return\x20instance_;\n}"
    return content

def generation_logic_freeinstance(project,name):
    content = "\n\nvoid\x20"+(name+"logic").title()+"::FreeInstance(){\n\x20\x20\x20\x20" + \
        "delete\x20instance_;\n\x20\x20\x20\x20instance_ = NULL;\n}"
    return content

def generation_logic_connect(project,name):
    content = "\n\nbool\x20"+(name+"logic").title()+"::On"+name.title() +"Connect" +\
        "(struct\x20server\x20*srv,const\x20int\x20socket){\n\n"+"\x20\x20\x20\x20return\x20true;\n}"
        
    return content

def generation_logic_message(project,name):
    content = "\n\nbool\x20"+(name+"logic").title()+"::On"+name.title() +"Message" +\
        "(struct\x20server\x20*srv,\x20const\x20int\x20socket,\x20const\x20void\x20*msg," + \
        "const\x20int\x20len){\n\n"+"\x20\x20\x20\x20return\x20true;\n}"
    return content

def generation_logic_close(project,name):
    content = "\n\nbool\x20"+(name+"logic").title()+"::On"+name.title() +"Close" +\
        "(struct\x20server\x20*srv,const\x20int\x20socket){\n\n"+"\x20\x20\x20\x20return\x20true;\n}"
        
    return content


def generation_logic_br_connect(project,name):
    content = "\n\nbool\x20"+(name+"logic").title()+"::OnBroadcastConnect" +\
        "(struct\x20server\x20*srv,\x20const\x20int\x20socket,\x20const\x20void\x20*msg," + \
        "const\x20int\x20len){\n\n"+"\x20\x20\x20\x20return\x20true;\n}"
        
    return content


def generation_logic_br_message(project,name):
    content = "\n\nbool\x20"+(name+"logic").title()+"::OnBroadcastMessage" +\
        "(struct\x20server\x20*srv,\x20const\x20int\x20socket,\x20const\x20void\x20*msg," + \
        "const\x20int\x20len){\n\n"+"\x20\x20\x20\x20return\x20true;\n}"
        
    return content


def generation_logic_br_close(project,name):
    content = "\n\nbool\x20"+(name+"logic").title()+"::OnBroadcastClose" +\
        "(struct\x20server\x20*srv,\x20const\x20int\x20socket){\n\n"+"\x20\x20\x20\x20return\x20true;\n}"
        
    return content

def generation_logic_init_time(project,name):    
    content = "\n\nbool\x20"+(name+"logic").title()+"::OnIniTimer" +\
        "(struct\x20server\x20*srv){\n\n"+"\x20\x20\x20\x20return\x20true;\n}"
        
    return content

def generation_logic_time_out(project,name):    
    content = "\n\nbool\x20"+(name+"logic").title()+"::OnTimeout" +\
        "(struct\x20server\x20*srv,\x20char\x20*id,\x20int\x20opcode,\x20int time){\n\n"+\
        "\x20\x20\x20\x20return\x20true;\n}"
        
    return content
    
def generation_logic_cpp(worksapce,project,file,name,path):
    global FILE_NAME_HEAD_LOGIC
    content = "#include\x20\""+FILE_NAME_HEAD_LOGIC+"\"\n#include\x20\"common.h\"\n\n" + \
        "namespace\x20"+project+"_logic{\n\n"+(name+"logic").title()+"*\n"+(name+"logic").title()+"::instance_=NULL;\n\n" + \
        generation_logic_construction(project,name) + "\n\n" + \
        generation_logic_destruct(project,name)+"\n\n"+generation_logic_init(project,name) + \
        generation_logic_getinstance(project,name)+"\n\n"+generation_logic_freeinstance(project,name) + \
        generation_logic_connect(project,name)+'\n\n' + generation_logic_message(project,name) + \
        generation_logic_close(project,name) + '\n\n' +generation_logic_br_connect(project,name) + \
        generation_logic_br_message(project,name) + '\n\n' +generation_logic_br_close(project,name) + \
        generation_logic_init_time(project,name) + '\n\n' + generation_logic_time_out(project,name) + \
        "\n\n}"+"\n\n"
        
        
        #写入文件
    FILE_NAME_CPP_LOGIC = name+"_logic.cc"
    write_file(path+FILE_NAME_CPP_LOGIC,content)
    
if __name__ == '__main__':
    print os.name
    sysstr = platform.system()
    if(platform.system()=="Darwin"):
        reload(sys)
        sys.setdefaultencoding('utf-8')
    print sys.getdefaultencoding()
    
    path = "./../plugins/msgsvc/"
    name = "msg"
    file = "msg"
    project = "msgsvc"
    workspace = "migfm"
    
    generation_init_head(workspace,project,file,name,path)
    generation_logic_head(workspace,project,file,name,path)
    generation_init_cpp(workspace,project,file,name,path)
    generation_logic_cpp(workspace,project,file,name,path)
