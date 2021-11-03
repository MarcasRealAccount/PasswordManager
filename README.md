# PasswordManager
A free open source no bs password manager.

# Build
Dependencies:
* [Premake](https://github.com/premake/premake-core)
* [VulkanSDK](https://vulkan.lunarg.com/)
* [OpenSSL](https://github.com/openssl/openssl)

Add `OPENSSL_LIB_CRYPTO_PATH` to environment variables pointint to where openssl's `libcrypto` library file is at.  
Add `OPENSSL_LIB_CRYPTO_SHARED_PATH` to environment variables to where openssl's `libcrypto` shared library file is at.  
Add `OPENSSL_INCLUDE` to environment variables to where openssl's include folder is at.  
If there isn't a `VULKAN_SDK` environment variable, make one pointing to the sdk folder on windows or `source setup-env.sh` script file inside the sdk folder.  
Run `premake5 <action>` where action is the build system you want to use.  
For windows you can use vs[version] for a visual studio build of the version like vs2019 or vs2022.  
For macosx you can use xcode4 and gmake2 (GNU Makefile).  
For linux you can use gmake2 (GNU Makefile).
