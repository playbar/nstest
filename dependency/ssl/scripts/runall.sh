#!/bin/sh
#
# 在MACOSX中，在openssl源码目录下使用
#

# set -v
set -e

if [ "$(stat -f %i .)" != "$(stat -f %i ../openssl-1.0.0e)" ]; then
  echo '此脚本仅在openssl-1.0.0e版本下调试通过'
  exit 1
fi

CONFIGSCRIPT=../scripts/Configure-darwin.sh

MAKELOG=../scripts/makelog
RUNLOG=../scripts/runlog

SRCDIRS=../scripts/srcdirs
SRCNAMES=../scripts/srcnames
SRCFILES=../scripts/srcfiles

SRCDIRS_CRYPTO=../scripts/srcdirs_crypto

HDRFILES_OPENSSL=../scripts/hdrfiles_openssl
HDRFILES_LOCAL=../scripts/hdrfiles_local
HDRFILES_COMMON=../scripts/hdrfiles_common
HDRFILES_DARWIN=../scripts/hdrfiles_darwin
HDRFILES_DARWIN_OPENSSL=../scripts/hdrfiles_darwin_openssl

DSRCDIR=../src
DINCDIR=../include
DINCDIR_OPENSSL=$DINCDIR/openssl
DINCDIR_LOCAL=../include-local
DINCDIR_COMMON=../include-common
DINCDIR_DARWIN=../include-darwin
DINCDIR_DARWIN_OPENSSL=../include-darwin/openssl
DINCDIR_WIN32=../include-win32
DINCDIR_WIN32_OPENSSL=../include-win32/openssl

OTHER_UNUSED_SRCNAMES="cversion.c"
TMPFILE=../scripts/tmpfile

if [ $(uname) != Darwin ]; then
  echo "必须在MACOSX中运行此脚本"
  exit 1
fi

if ! [ -e Configure ]; then
  echo "必须在openssl源码根目录下运行此脚本"
  exit 1
fi

if ! [ -e $CONFIGSCRIPT ]; then
  echo "脚本$CONFIGSCRIPT不存在"
  exit 1
fi

echo "清理工作区"

rm -rf $MAKELOG $RUNLOG $TMPFILE \
       $SRCDIRS $SRCNAMES $SRCFILES $SRCDIRS_CRYPTO \
       $HDRFILES_LOCAL $HDRFILES_COMMON $HDRFILES_OPENSSL \
       $HDRFILES_DARWIN_OPENSSL $HDRFILES_DARWIN \
       $HDRFILES_WIN32_OPENSSL $HDRFILES_WIN32

rm -rf $DINCDIR_WIN32_OPENSSL $DINCDIR_WIN32 \
       $DINCDIR_DARWIN_OPENSSL $DINCDIR_DARWIN \
       $DINCDIR_LOCAL $DINCDIR_COMMON $DINCDIR_OPENSSL $DINCDIR \
       $DSRCDIR


for i in $DSRCDIR \
         $DINCDIR $DINCDIR_OPENSSL \
         $DINCDIR_DARWIN $DINCDIR_DARWIN_OPENSSL \
         $DINCDIR_LOCAL $DINCDIR_COMMON; do
  if ! [ -e $i ]; then
    mkdir $i
  fi
done

echo "配置及编译"

echo "$CONFIGSCRIPT"
$CONFIGSCRIPT >> $RUNLOG

echo "make depend"
make depend >> $RUNLOG

echo "make build_libs"
make build_libs > $MAKELOG

echo "生成$SRCFILES"
cat $MAKELOG | sed -nE 's/^cc.* ([^ ]+.c)$/\1/p' | sort -u > $SRCNAMES

cat /dev/null > $TMPFILE
for i in ssl crypto; do
  for j in $(find $i -name '*.c'); do
    name=$(basename $j)

    # 排除由$OTHER_UNUSED_SRCNAMES定义的文件
    for k in $OTHER_UNUSED_SRCNAMES; do
      if [ $k = $name ]; then
        echo "$name excluded"
        continue 2;
      fi
    done

    # 排除以test.c结尾的文件
    if [[ $name =~ '^.*test.c$' ]]; then
      echo "$name excluded"
      continue;
    fi

    # 排除不在$SRCNAMES中的文件
    if ! grep -qx $name $SRCNAMES; then
      echo "$name excluded"
      continue;
    fi

    echo $j >> $TMPFILE
  done
done
cat $TMPFILE | sort -u > $SRCFILES

echo "生成$SRCDIRS"
cat $SRCFILES | sed -nE 's/^(.*)\/[^ ]+.c$/\1/p' | sort -u > $SRCDIRS

echo "生成$SRCDIRS_CRYPTO"
cat $SRCDIRS | sed -nE 's/^(crypto\/[^/]+)/\1/p' | sort -u > $SRCDIRS_CRYPTO

echo "生成$HDRFILES_OPENSSL"
cat /dev/null > $TMPFILE
for i in $(find include/openssl -name '*.h'); do
  # 排除opensslconf.h
  if echo "$i" | grep -qE '\/opensslconf\.h$'; then
    echo "$i excluded"
    continue
  fi
  echo $i >> $TMPFILE
done
echo "crypto/comp/comp.h" >> $TMPFILE
cat $TMPFILE | sort -u > $HDRFILES_OPENSSL

echo "生成$HDRFILES_LOCAL"
cat /dev/null > $TMPFILE
for i in $(cat $SRCDIRS_CRYPTO) ssl engines; do
  for j in $(find $i -name '*_locl.h') \
           $(find $i -name '*_lcl.h') \
           $(find $i -name '*_int.h'); do
    echo $j >> $TMPFILE
  done
done

MORE_FILES="crypto/des/rpc_des.h \
            crypto/des/des_ver.h \
            crypto/objects/obj_xref.h \
            crypto/objects/obj_dat.h \
            crypto/comp/comp.h \
            crypto/des/spr.h \
            crypto/des/ncbc_enc.c \
            crypto/conf/conf_def.h \
            crypto/bn/bn_prime.h \
            crypto/asn1/charmap.h \
            crypto/x509v3/ext_dat.h"

for i in $MORE_FILES; do
  echo $i >> $TMPFILE
done

cat $TMPFILE | sort -u > $HDRFILES_LOCAL

echo "生成$HDRFILES_COMMON"
cat /dev/null > $TMPFILE
MORE_FILES="crypto/cryptlib.h \
            crypto/md32_common.h \
            crypto/o_dir.h \
            crypto/o_str.h \
            crypto/o_time.h \
            e_os.h"

for i in $MORE_FILES; do
  echo $i >> $TMPFILE
done
cat $TMPFILE | sort -u > $HDRFILES_COMMON

echo "生成$HDRFILES_DARWIN"
echo "crypto/LPdir_unix.c" > $TMPFILE
echo "crypto/buildinf.h" >> $TMPFILE
cat $TMPFILE | sort -u > $HDRFILES_DARWIN

echo "生成$HDRFILES_DARWIN_OPENSSL"
echo "crypto/opensslconf.h" > $TMPFILE
cat $TMPFILE | sort -u > $HDRFILES_DARWIN_OPENSSL

# 复制文件

echo "复制到$DSRCDIR"
for i in $(cat $SRCFILES); do
  cp -rv $i $DSRCDIR >> $RUNLOG
done

echo "复制到$DINCDIR_OPENSSL"
for i in $(cat $HDRFILES_OPENSSL); do
  cp -rv $i $DINCDIR_OPENSSL >> $RUNLOG
done

echo "复制到$DINCDIR_DARWIN"
for i in $(cat $HDRFILES_DARWIN); do
  cp -rv $i $DINCDIR_DARWIN >> $RUNLOG
done

echo "复制到$DINCDIR_DARWIN_OPENSSL"
for i in $(cat $HDRFILES_DARWIN_OPENSSL); do
  cp -rv $i $DINCDIR_DARWIN_OPENSSL >> $RUNLOG
done

echo "复制到$DINCDIR_LOCAL"
for i in $(cat $HDRFILES_LOCAL); do
  cp -rv $i $DINCDIR_LOCAL >> $RUNLOG
done

echo "复制到$DINCDIR_COMMON"
for i in $(cat $HDRFILES_COMMON); do
  cp -rv $i $DINCDIR_COMMON >> $RUNLOG
done

# 将.c文件中所有形如#include "file.h"的语句，如果文件file.h存在于../include/openssl目录之下的话，修改为#include <openssl/file.h>。
echo '修改头文件包含语句 include "file.h" -> include <openssl/file.h>'
SEDCMDS=
for i in $(ls $DINCDIR_OPENSSL) $(ls $DINCDIR_DARWIN_OPENSSL); do
  NAMEPAT=$(echo $i | sed -nE 's/\.h$/\\\.h/p')
  SEDCMDS=$(printf "%s\n%s\n%s" "$SEDCMDS" "s/^(#[[:space:]]*include[[:space:]]+)\"($NAMEPAT)\"/\1<openssl\/\2>/p" "t")
done
SEDCMDS=$(printf "%s\n%s" "$SEDCMDS" "p")
echo "SEDCMDS=$SEDCMDS" >> $RUNLOG

for i in $(ls $DSRCDIR); do
  cat $DSRCDIR/$i | sed -nE -e "$SEDCMDS" > $TMPFILE
  if ! diff -q $DSRCDIR/$i $TMPFILE >> $RUNLOG 2>>$RUNLOG; then
    cp $TMPFILE $DSRCDIR/$i
    echo "$DSRCDIR/$i modified"
  fi
done

if [ -e $DSRCDIR/wp_dgst.c ]; then
  echo "修改wp_dgst.c，添加一个函数声明"
  sed -nE -e '/^int WHIRLPOOL_Init/ i\
void WHIRLPOOL_BitUpdate(WHIRLPOOL_CTX *c,const void *_inp,size_t bits);
p' $DSRCDIR/wp_dgst.c > $TMPFILE
  cp $TMPFILE $DSRCDIR/wp_dgst.c
  echo "$DSRCDIR/wp_dgst modified"
fi

echo "为WIN32平台的编译将include winsock.h修改为include winsock2.h"
for i in $DINCDIR_COMMON/e_os.h \
         $DINCDIR_OPENSSL/dtls1.h; do
  if [ -e $i ]; then
    sed -nE -e 's/^(#[[:space:]]*include[[:space:]]*)<winsock\.h>/\1<winsock2\.h>/p
t
p' $i > $TMPFILE
    cp $TMPFILE $i
    echo "$i modified"
  fi
done

echo "清理"
make clean >> $RUNLOG

echo "结束"


