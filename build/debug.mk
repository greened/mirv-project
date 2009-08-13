ifndef DEBUG_MK
DEBUG_MK := 1

ifdef MDEBUG
debug = $(info [DEBUG] $(1))
endif

endif
