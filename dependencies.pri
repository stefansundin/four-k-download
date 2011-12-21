######################################################################
#
# Apply project dependencies
#
######################################################################

win32 {
	DEP_LIB_PREF=lib
	DEP_LIB_EXT=a
} else {
	DEP_LIB_PREF=lib
	DEP_LIB_EXT=a
}

for(library, DEPENDENCY_LIBS) {
    PRE_TARGETDEPS += $${OUTPUT_DIR}/lib/$${DEP_LIB_PREF}$${library}.$${DEP_LIB_EXT}
}