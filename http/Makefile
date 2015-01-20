
###############################################################################
## 编译器
COMPILER = g++
COMPILERC = gcc

## 编译选项
## for debug -DROOM_DEBUG_MCACHE
CPP_FLAG = -g -Wall -w -fPIC -c -D_LINUX -DDEBUG -D_DEBUG -D_REENTRANT -D_GNU_SOURCE -D__DEBUG_CORE__ -DFCGI_STD  #-DCHECK_HEADER-DTEST
RCPP_FLAG = -g -Wall -w -fPIC -c -D_LINUX -DDEBUG -D_DEBUG -D_REENTRANT -D_GNU_SOURCE -DFCGI_STD #-DTEST
## for release
##CPP_FLAG = -O3 -Wall -s -w -fPIC -c -D_LINUX -DNDEBUG -D_REENTRANT -D_GNU_SOURCE -DFCGI_STD

## 链接选项
## for debug
LINK_FLAGS = -g  -fPIC
## for release
## LINK_FLAGS = -s -O3 -shared -fPIC

LIB_DIR = ./../../library
BASE_DIR = ./../pub
## 头文件
INCLUDE_PATHS = -I$(LIB_DIR)/public/ -I /usr/local/libmemcached-1.0.7/include\
		-I/usr/local/lib -I$(LIB_DIR)/third_party/hiredis -I$(BASE_DIR)/\
		-I$(LIB_DIR)/third_party/breakpad/src

## 库
LINK_PATHS =  -L/usr/local/libmemcached-1.0.7/lib/ -L/usr/lib64/mysql
LINK_LIBRARYS = -lmemcached -lmemcachedutil -lrt -lpthread  -liconv -lfcgi++ -lmysqlclient -lexpat -lcurl\
					./../lib/libbreakpad_client.a ./../lib/libhiredis.a $(LINK_CORE_PATH)

# 编译
COMPILE = $(COMPILER) $(CPP_FLAG) $(INCLUDE_PATHS) -o $@
RCOMPILE = $(COMPILER) $(RCPP_FLAG) $(INCLUDE_PATHS) -o $@

# 链接
LINK = $(COMPILER) $(LINK_FLAGS) $(LINK_PATHS) $? -o $(OUTPUT_FILE) $(LINK_LIBRARYS)
LINKALL = $(COMPILER) $(LINK_FLAGS) $(LINK_PATHS) $?  $(LINK_LIBRARYS)











POBJ =  mig_log.o core_interface.o
OUTPUT_FILE = /var/www/cgi-bin/buddha
#############################################################################################################################
all: clean proc delete-temp-file

proc: otherproc userproc findproc buildproc bookproc activitiesproc introproc robotproc

	
######################otherproc###############################################
OTHERPROC_PATH = /other
OTJERPROC_VERSION = /1
otherproc:checkapi.fcgi

checkapi.fcgi: checkapi.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(OTHERPROC_PATH)$(OTJERPROC_VERSION)/checkapi.fcgi
	
### 编译

checkapi.o:
	$(COMPILE) .$(OTHERPROC_PATH)$(OTJERPROC_VERSION)/checkapi/checkapi.cc
	
######################robotproc###############################################
ROBOTPROC_PATH = /robot
OTJERPROC_VERSION = /1
robotproc:bookcollection.fcgi

bookcollection.fcgi: bookcollection.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(ROBOTPROC_PATH)$(OTJERPROC_VERSION)/bookcollection.fcgi
	
### 编译

bookcollection.o:
	$(COMPILE) .$(ROBOTPROC_PATH)$(OTJERPROC_VERSION)/bookcollection/bookcollection.cc
	
######################buildproc###############################################
BUILDPROC_PATH = /build
OTJERPROC_VERSION = /1
buildproc:nearbuild.fcgi searchtype.fcgi summary.fcgi

nearbuild.fcgi: nearbuild.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(BUILDPROC_PATH)$(OTJERPROC_VERSION)/nearbuild.fcgi
	
searchtype.fcgi: searchtype.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(BUILDPROC_PATH)$(OTJERPROC_VERSION)/searchtype.fcgi
	
summary.fcgi: summary.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(BUILDPROC_PATH)$(OTJERPROC_VERSION)/summary.fcgi
	
### 编译

nearbuild.o:
	$(COMPILE) .$(BUILDPROC_PATH)$(OTJERPROC_VERSION)/nearbuild/nearbuild.cc
	
searchtype.o:
	$(COMPILE) .$(BUILDPROC_PATH)$(OTJERPROC_VERSION)/searchtype/searchtype.cc
	
summary.o:
	$(COMPILE) .$(BUILDPROC_PATH)$(OTJERPROC_VERSION)/summary/summary.cc
	
######################findproc###############################################
FINDPROC_PATH = /find
OTJERPROC_VERSION = /1
findproc:broad.fcgi building.fcgi book.fcgi intro.fcgi

broad.fcgi: broad.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(FINDPROC_PATH)$(OTJERPROC_VERSION)/broad.fcgi
	
building.fcgi: building.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(FINDPROC_PATH)$(OTJERPROC_VERSION)/building.fcgi

book.fcgi: book.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(FINDPROC_PATH)$(OTJERPROC_VERSION)/book.fcgi
	
intro.fcgi: intro.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(FINDPROC_PATH)$(OTJERPROC_VERSION)/intro.fcgi
### 编译

broad.o:
	$(COMPILE) .$(FINDPROC_PATH)$(OTJERPROC_VERSION)/broad/broad.cc
	
building.o:
	$(COMPILE) .$(FINDPROC_PATH)$(OTJERPROC_VERSION)/building/building.cc
	
book.o:
	$(COMPILE) .$(FINDPROC_PATH)$(OTJERPROC_VERSION)/book/book.cc
	
intro.o:
	$(COMPILE) .$(FINDPROC_PATH)$(OTJERPROC_VERSION)/intro/intro.cc
	
######################userproc###############################################
USERPROC_PATH = /user
OTJERPROC_VERSION = /1
userproc:quicklogin.fcgi bdbindpush.fcgi thirdlogin.fcgi

quicklogin.fcgi: quicklogin.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(USERPROC_PATH)$(OTJERPROC_VERSION)/quicklogin.fcgi

thirdlogin.fcgi: thirdlogin.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(USERPROC_PATH)$(OTJERPROC_VERSION)/thirdlogin.fcgi
	
bdbindpush.fcgi: bindpush.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(USERPROC_PATH)$(OTJERPROC_VERSION)/bdbindpush.fcgi
	
### 编译

quicklogin.o:
	$(COMPILE) .$(USERPROC_PATH)$(OTJERPROC_VERSION)/quicklogin/quicklogin.cc
	
bindpush.o:
	$(COMPILE) .$(USERPROC_PATH)$(OTJERPROC_VERSION)/bindpush/bindpush.cc

thirdlogin.o:
	$(COMPILE) .$(USERPROC_PATH)$(OTJERPROC_VERSION)/thirdlogin/thirdlogin.cc
	
	
######################introproc###############################################
INTROPROC_PATH = /intro
OTJERPROC_VERSION = /1
introproc: introsearchtype.fcgi  art.fcgi hisathr.fcgi

art.fcgi: art.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(INTROPROC_PATH)$(OTJERPROC_VERSION)/art.fcgi

hisathr.fcgi: hisathr.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(INTROPROC_PATH)$(OTJERPROC_VERSION)/hisathr.fcgi

introsearchtype.fcgi: introsearchtype.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(INTROPROC_PATH)$(OTJERPROC_VERSION)/introsearchtype.fcgi
### 编译
art.o:
	$(COMPILE) .$(INTROPROC_PATH)$(OTJERPROC_VERSION)/art/art.cc
	
hisathr.o:
	$(COMPILE) .$(INTROPROC_PATH)$(OTJERPROC_VERSION)/hisathr/hisathr.cc

introsearchtype.o:
	$(COMPILE) .$(INTROPROC_PATH)$(OTJERPROC_VERSION)/searchtype/searchtype.cc
	

######################activitiesproc###############################################
ACTIVITYRPROC_PATH = /activities
OTJERPROC_VERSION = /1
activitiesproc:summary.fcgi activities.fcgi

summary.fcgi: summary.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(ACTIVITYRPROC_PATH)$(OTJERPROC_VERSION)/summary.fcgi

activities.fcgi: activities.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(ACTIVITYRPROC_PATH)$(OTJERPROC_VERSION)/activities.fcgi
### 编译
summary.o:
	$(COMPILE) .$(ACTIVITYRPROC_PATH)$(OTJERPROC_VERSION)/summary/summary.cc
	
activities.o:
	$(COMPILE) .$(ACTIVITYRPROC_PATH)$(OTJERPROC_VERSION)/activities/activities.cc
	
	
######################bookproc###############################################
BOOKRPROC_PATH = /book
OTJERPROC_VERSION = /1
bookproc:searchtype.fcgi booksummary.fcgi booklist.fcgi wantgetbook.fcgi chapterlist.fcgi

searchtype.fcgi: searchtype.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(BOOKRPROC_PATH)$(OTJERPROC_VERSION)/searchtype.fcgi

booksummary.fcgi: booksummary.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(BOOKRPROC_PATH)$(OTJERPROC_VERSION)/booksummary.fcgi
	
booklist.fcgi: booklist.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(BOOKRPROC_PATH)$(OTJERPROC_VERSION)/booklist.fcgi

wantgetbook.fcgi: wantgetbook.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(BOOKRPROC_PATH)$(OTJERPROC_VERSION)/wantgetbook.fcgi

chapterlist.fcgi: chapterlist.o $(POBJ)
	$(LINKALL) -o $(OUTPUT_FILE)$(BOOKRPROC_PATH)$(OTJERPROC_VERSION)/chapterlist.fcgi
### 编译

searchtype.o:
	$(COMPILE) .$(BOOKRPROC_PATH)$(OTJERPROC_VERSION)/searchtype/searchtype.cc
	
booksummary.o:
	$(COMPILE) .$(BOOKRPROC_PATH)$(OTJERPROC_VERSION)/booksummary/booksummary.cc

booklist.o:
	$(COMPILE) .$(BOOKRPROC_PATH)$(OTJERPROC_VERSION)/booklist/booklist.cc

wantgetbook.o:
	$(COMPILE) .$(BOOKRPROC_PATH)$(OTJERPROC_VERSION)/wantgetbook/wantgetbook.cc

chapterlist.o:
	$(COMPILE) .$(BOOKRPROC_PATH)$(OTJERPROC_VERSION)/chapterlist/chapterlist.cc

	

clean:
	-rm -f *.o
	-rm -f $(OUTPUT_FILE)./*.fcgi

delete-temp-file:
	-rm -f *.o

############public###############################
mig_log.o:
	$(COMPILE) $(LIB_DIR)/public/log/mig_log.cc

core_interface.o:
	$(COMPILE) $(LIB_DIR)/public/comm/core_interface.cc
