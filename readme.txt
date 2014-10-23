说明
_______________________________________________________________________________________________________________________________
_______________________________________________________________________________________________________________________________

config.lua 配置文件，和程序同级目录
_______________________________________________________________________________________________________________________________

lua c++互相调用功能：

c++:
main函数中：
#include "plua.h"
#include "luatest.h"

PLua* p = new PLua("test.lua");
p->Call("test1", ""); // 测试函数1调用
CScriptTest test;
test.m_id = 1;
test.m_name = "test";
std::cout << "c++输出1：" << test.m_id << std::endl;
p->Call("test2", "t", &test);
std::cout << "c++输出2：" << test.m_id << std::endl;

程序文件：
script_test.h 在swig_wrap.i中包含的文件，表示所有开放给lua的程序接口，程序内部不使用
luatest.h 正常类头文件，程序内部使用
luatest.cpp 正常类实现

lua:
script/test.lua中：

testnum = 10

print("lua 测试输出")

function test1()
	print("lua test1被调用")
end

function test2(t)
	t:ScriptTest()
	t:Change()
	t.m_id = t.m_id + 100;
	print("lua中c程序返回结果：",t:ScriptTest())
	print("lua cpp函数test调用完成")
end

_______________________________________________________________________________________________________________________________

