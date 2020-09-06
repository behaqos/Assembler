.name           "42"
.comment        "Just a basic Winner Program"

entree:
    ld	%0,r5 # offset 0
    live	%42		# offset = 0, len == 5
	ld	%0,r5       # offset = 5, len == 7
test:  # offset = 12
	ld	%:end,r5 # offset = 12, len == 7
end:  #  offset 19
