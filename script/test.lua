
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

print("lua �������")

function test1()
	print("lua test1������")
end

function test2(t)
	t:ScriptTest()
	t:Change()
	t.m_id = t.m_id + 100;
	print("lua��c���򷵻ؽ����",t:ScriptTest())
	print("lua cpp����test�������")
end
