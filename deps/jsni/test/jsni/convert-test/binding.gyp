{
  "targets": [
    {
      "target_name": "addon",
      "sources": [ "hello.cc"],
      "cflags" : ["-fprofile-arcs -ftest-coverage"],
      "ldflags" : ["-lgcov --coverage"],
    }
  ]
}
