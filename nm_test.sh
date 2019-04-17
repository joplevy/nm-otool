rm -f expected result
nm $@ > expected
./ft_nm $@ > result
diff expected result
