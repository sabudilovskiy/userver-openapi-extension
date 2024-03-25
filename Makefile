.PHONY: check-git-status
check-git-status:
	@echo "Checking if all files are committed to git..."
	@if [ -n "$$(git status --porcelain)" ]; then \
		echo "The following files are not committed:"; \
		git status --short; \
		echo "Please commit all changes and try again."; \
		git diff --color | cat; \
		exit 1; \
	else \
		echo "All files are committed to git."; \
	fi

.PHONY: add-eol
add-eol:
	@find $(P) -type f | while read file; do \
        if ! tail -c1 "$$file" | grep -q "^$$"; then \
            echo >> "$$file"; \
        fi \
    done

.PHONY: add-eol-root
add-eol-root:
	@find . -maxdepth 1 -type f | while read file; do \
		if ! tail -c1 "$$file" | grep -q "^$$"; then \
			echo >> "$$file"; \
		fi \
    done

.PHONY: instal-compiler
install-compiler:
	@if [ "$(compiler)" = "clang" ]; then \
            wget https://apt.llvm.org/llvm.sh; \
            chmod +x llvm.sh; \
            sudo ./llvm.sh $(version); \
            rm llvm.sh;\
    elif [ "$(compiler)" = "gcc" ]; then \
      	echo "hi linux"; \
  	else \
      echo "Unknown compiler"; \
  	fi

.PHONY: find-cxx-compiler
find-cxx-compiler:
	@if [ "$(compiler)" = "clang" ]; then \
        echo "/usr/bin/clang-$(version)"; \
      elif [ "$(compiler)" = "gcc" ]; then \
        echo "Gcc compiler" >&2; \
      else \
        echo "Unknown compiler" >&2; \
      fi

.PHONY: find-c-compiler
find-c-compiler:
	@if [ "$(compiler)" = "clang" ]; then \
        echo "/usr/bin/clang-$(version)"; \
      elif [ "$(compiler)" = "gcc" ]; then \
        echo "Gcc compiler" >&2;  \
      else \
        echo "Unknown compiler" >&2;  \
      fi
