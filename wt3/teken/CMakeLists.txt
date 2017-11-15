INCLUDE_DIRECTORIES(/usr/local/wt/include)
#LINK_DIRECTORIES(/usr/local/lib)

ADD_EXECUTABLE(teken
teken.cpp
)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")

# For FastCGI deployment:
##TARGET_LINK_LIBRARIES(myprog.wt
##wtfcgi wt someotherlib
##)

# Or, for built-in httpd deployment:
TARGET_LINK_LIBRARIES(teken
  wthttp wt pthread boost_signals
)

