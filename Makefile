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
      	sudo apt install -y g++-$(version); \
      	sudo apt install -y gcc-$(version); \
  	else \
      echo "Unknown compiler" >&2; \
  	fi

.PHONY: find-cxx-compiler
find-cxx-compiler:
	@if [ "$(compiler)" = "clang" ]; then \
        echo "/usr/bin/clang++-$(version)"; \
      elif [ "$(compiler)" = "gcc" ]; then \
        echo "/usr/bin/g++-$(version)"; \
      else \
        echo "Unknown compiler" >&2; \
      fi

.PHONY: find-c-compiler
find-c-compiler:
	@if [ "$(compiler)" = "clang" ]; then \
        echo "/usr/bin/clang-$(version)"; \
      elif [ "$(compiler)" = "gcc" ]; then \
        echo "/usr/bin/gcc-$(version)"; \
      else \
        echo "Unknown compiler" >&2;  \
      fi

.PHONY: format
format:
	python3.10 scripts/generate_all_headers.py library/uopenapi uopenapi
	python3.10 scripts/format_includes.py library boost uopenapi checks
	find checks -name '*pp' -type f | xargs clang-format-15 -i
	find library -name '*pp' -type f | xargs clang-format-15 -i
	make add-eol P=checks
	make add-eol P=library
	make add-eol P=CMake
	make add-eol P=.github
	make add-eol P=scripts
	make add-eol-root
