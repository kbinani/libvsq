# libvsq

libvsqは, VOCALOID, VOCALOID2 用の VSQ ファイル (*.vsq) の読み書き, 編集を行うための C++ ライブラリです.

## ライセンス

修正BSDライセンスです.

## ライブラリ・リファレンス

http://ci.kbinani.info/job/libvsq-devel/doxygen/

## ビルド

このプロジェクトは CMake を使っています. CMake の通常の手順で `cmake .`, `make` でビルドできます.

## 開発のススメ

### Unit test

`tests` ディレクトリにテストコードがあります.
テストコードは以下の手順でビルド, 実行できます.
```
cd tests
cmake .
make
./libvsqtest
```

### カバレッジの計測

Unit test のカバレッジを計測するには, 以下のように cmake コマンドに `-DCOVERAGE=true` を付けて再ビルドします.
```
cmake . -DCOVERAGE=true
make
cd tests
cmake . -DCOVERAGE=true
make
./libvsqtest
cd ../
gcovr -r . --exclude=tests
```

## 商標など

VOCALOID はヤマハ株式会社の登録商標です.
