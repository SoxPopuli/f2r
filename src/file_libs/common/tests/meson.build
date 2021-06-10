
common_test_files = [
  'endian_test.cpp',
  'str_array_test.cpp'
]

test_suites = [ 'common', 'file' ]
foreach test : common_test_files
  parts = test.split('.')
  exe = executable(parts[0], files(test), 
    dependencies: common
  )
  test(parts[0], exe,
    suite: test_suites
  )
endforeach
