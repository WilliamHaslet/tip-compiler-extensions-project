testFun(arg1, arg2, arg3, arg4, arg5, arg6, arg7)
{
    return arg1 + 1;
}

testFun2(arg1, arg2, arg3, arg4, arg5, arg6, arg7)
{
    return arg1 + 1;
}

perf() {
    var x,y,z;
    y = 5;
    for (x: 1 .. 100){
        z++;
    }
    return y;
}

perf2() {
    var x, y;
    y = 0;
    for (x : 1 .. 100){
        y++;
    }
    return y;
}

perf3(a) {
    var x,y,z;
    for (y : 0 .. #a) {
        if (x>#a) {
            a[y] = x;
            x = x/2; 
        } else {
            a[y] = x;
            x = x/2; 
            z++;
        }
    }
    return z*a[#a-1];
}

main()
{
    var test, i, c, j, a;
    a = [1,2,3,4,5,6,7,8,9];
    test = 0;

    for (i : 0 .. 1000000)
    {
        test = test + testFun(testFun2(i % 2, i % 10, 4, 2, i, i, 40954), i % 10, 4, 2, i, i, 40954);
        c = perf();
        j = perf2();
        j = perf3(a);
    }

    return 0;
}
