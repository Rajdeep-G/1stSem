flag = "iMhceaSlocttaPepCrmoapyn"

# Little-Endian
flag_little_endian = ''.join(reversed(flag))
ascii_little_endian = ''.join([chr(int(flag_little_endian[i:i+2], 16)) for i in range(0, len(flag_little_endian), 2)])

# Big-Endian
ascii_big_endian = ''.join([chr(int(flag[i:i+2], 16)) for i in range(0, len(flag), 2)])

print("Little-Endian Hex:", ' '.join([flag_little_endian[i:i+2] for i in range(0, len(flag_little_endian), 2)]))
print("Big-Endian Hex:", ' '.join([flag[i:i+2] for i in range(0, len(flag), 2)]))

print("\nLittle-Endian:", ascii_little_endian)
print("Big-Endian:", ascii_big_endian)
