all: hello-gettext
	echo "Done"

hello-gettext: hello-gettext.cpp
	g++ -o hello-gettext hello-gettext.cpp

clean:
	rm -f hello-gettext hello-gettext.o

hello.pot: hello-gettext.cpp
	xgettext --package-name hello-gettext --package-version 1.1 --default-domain hello --output hello.pot hello-gettext.cpp

hello.po: hello.pot
	msginit --no-translator --locale zh_CN.UTF-8 --output-file hello.po --input hello.pot

hello.mo: hello.po
	msgfmt --check --verbose --output-file ./zh_CN.UTF-8/LC_MESSAGES/hello.mo hello.po

# 1. Generate .pot
# xgettext --package-name hello-gettext --package-version 1.1 --default-domain hello --output hello.pot hello-gettext.cpp
#
# 2. Generate .po
# msginit --no-translator --locale zh_CN.UTF-8 --output-file hello.po --input hello.pot
#
# 3. Edit po and translate the messsages
#
# 4. Generate .mo
# msgfmt --check --verbose --output-file ./zh_CN.UTF-8/LC_MESSAGES/hello.mo hello.po
#
# 5. Set locale environment
# export LANG=zh_CN.UTF-8
# export LANGUAGE=zh_CN.UTF-8
#
# 6. Run your program
# ./hello-gettext
