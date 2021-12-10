#!/bin/zsh

file='funInline.sip'
testFolder='test/optimizer/'
args='-do=_ -or=1'
args2='-do=funinline -or=1'

echo test > a

echo with optimization

./bin/build.sh $args $testFolder$file

for i in {0..2}
do 
    time ./$file > a
done

echo no optimization

./bin/build.sh $args2 $testFolder$file

for i in {0..2}
do 
    time ./$file > a
done

rm $testFolder$file.bc
rm $file
rm a

#./build/src/tipc -do=_ -or=1 -asm test/optimizer/globalDeadElim.sip
