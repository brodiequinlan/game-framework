pushd .. 
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars32.bat"
msbuild bq_game_lib.sln /p:Configuration=Release /p:Platform="x64"