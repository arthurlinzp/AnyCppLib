add_rules("mode.debug", "mode.release", "mode.releasedbg")

if is_plat("windows") then
    add_cxflags("/utf-8", {force = true}) -- MSVC
else
    add_cxflags("-finput-charset=UTF-8", "-fexec-charset=UTF-8", {force = true}) -- GCC/Clang
end

option("devlibs")
    set_default(true)
	add_includedirs("$(projectdir)/../../__devLibs_trdlp/inc")
    if is_plat("windows") then
        add_linkdirs("$(projectdir)/../../__devLibs_trdlp/libs")
    elseif is_arch("arm64-v8a") then
        add_linkdirs("$(projectdir)/../../__devLibs_trdlp/libs_4linux/aarch64/libs_x64")
        add_linkdirs("$(projectdir)/../../__devLibs_trdlp/libs_4linux/aarch64/lib3rds_x64")
    else
        add_linkdirs("$(projectdir)/../../__devLibs_trdlp/libs_4linux/x86_64/libs_x64")
        add_linkdirs("$(projectdir)/../../__devLibs_trdlp/libs_4linux/x86_64/lib3rds_x64")
    end
option_end()

target("AnyCppLib")
    set_kind("binary")
    add_options("devlibs")

    add_includedirs("include")
    add_files("src/*.cpp")

    add_links("DlpULog")
    
    -- 自动生成 compile_commands.json 帮助代码补全跳转
	after_build(function (target)
		import("core.base.task")
		task.run("project", {kind = "compile_commands", outputdir = ".vscode"})
	end)
target_end()


--
-- If you want to known more usage about xmake, please see https://xmake.io
--
-- ## FAQ
--
-- You can enter the project directory firstly before building project.
--
--   $ cd projectdir
--
-- 1. How to build project?
--
--   $ xmake
--
-- 2. How to configure project?
--
--   $ xmake f -p [macosx|linux|iphoneos ..] -a [x86_64|i386|arm64 ..] -m [debug|release]
--
-- 3. Where is the build output directory?
--
--   The default output directory is `./build` and you can configure the output directory.
--
--   $ xmake f -o outputdir
--   $ xmake
--
-- 4. How to run and debug target after building project?
--
--   $ xmake run [targetname]
--   $ xmake run -d [targetname]
--
-- 5. How to install target to the system directory or other output directory?
--
--   $ xmake install
--   $ xmake install -o installdir
--
-- 6. Add some frequently-used compilation flags in xmake.lua
--
-- @code
--    -- add debug and release modes
--    add_rules("mode.debug", "mode.release")
--
--    -- add macro definition
--    add_defines("NDEBUG", "_GNU_SOURCE=1")
--
--    -- set warning all as error
--    set_warnings("all", "error")
--
--    -- set language: c99, c++11
--    set_languages("c99", "c++11")
--
--    -- set optimization: none, faster, fastest, smallest
--    set_optimize("fastest")
--
--    -- add include search directories
--    add_includedirs("/usr/include", "/usr/local/include")
--
--    -- add link libraries and search directories
--    add_links("tbox")
--    add_linkdirs("/usr/local/lib", "/usr/lib")
--
--    -- add system link libraries
--    add_syslinks("z", "pthread")
--
--    -- add compilation and link flags
--    add_cxflags("-stdnolib", "-fno-strict-aliasing")
--    add_ldflags("-L/usr/local/lib", "-lpthread", {force = true})
--
-- @endcode
--

