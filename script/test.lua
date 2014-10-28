
testnum = 10

t = {
	t1 = 20,
	t2 = {
		t3 = 30,
		t4 = {
			t5 = 40,
		},
	},
}

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
