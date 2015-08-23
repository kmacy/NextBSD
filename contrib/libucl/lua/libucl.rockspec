package="libucl"
version="0.7.3-1"
source = {
  url = "https://github.com/downloads/vstakhov/libucl/libucl-0.7.3.tar.gz",
  md5 = "@MD5@",
  dir = "libucl-0.7.3"
}
description = {
  summary = "UCL - json like configuration language",
  detailed = [[
      UCL is heavily infused by nginx configuration as the example 
      of a convenient configuration system. 
      However, UCL is fully compatible with JSON format and is able 
      to parse json files. 
   ]],
  homepage = "http://github.com/vstakhov/libucl/",
  license = ""
}
dependencies = {
  "lua >= 5.1"
}
build = {
  type = "command",
  build_command = "LUA=$(LUA) CPPFLAGS=-I$(LUA_INCDIR) ./configure --prefix=$(PREFIX) --libdir=$(LIBDIR) --datadir=$(LUADIR) && make clean && make",
  install_command = "make install"
}
