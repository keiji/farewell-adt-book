= はじめに

このたびはAndroid Studio完全移行ガイドを手にとっていただき、ありがとうございます。

まずはじめに、本書が題材にしているAndroidアプリ開発の@<kw>{IDE, 統合開発環境}、「Android Studio」と「@<kw>{ADT, Android Developer Tools}」について解説します。

//image[as_adt_icons][Android Studio（左）とADT（右）][scale=0.12]{
//}

「Android Studio」は、Google I/O 2013で発表された「IntelliJ IDEA@<fn>{intellij}」ベースのIDEです。

一方、@<kw>{ADT,Android Developer Tools}は、2007年にAndroidが発表されて以来、普及してきたEclipseベースにのIDEです。
当初はプラグインの形態で配布されていましたが、中期以降は、Eclipseに組み込まれた状態（単体版）で配布されていました。

//footnote[intellij][IntelliJ IDEAは、チェコJetBrains社が開発、配布している統合開発環境です。Android Studioは「IntelliJ IDEA Community Edition」をベースに開発されています]

==== Time to Migrate
2015年1月、Android Studioのバージョンが1.0になると「公式開発環境」の冠はADTからAndroid Studioに移され、ADTは再びプラグインによる提供となりました。

更に米Google社は、それから半年経つか経たないかのうちに、2015年末でADTの開発やサポートを打ち切ることをアナウンスしました。

 * An update on Eclipse Android Developer Tools
 ** http://android-developers.blogspot.jp/2015/06/an-update-on-eclipse-android-developer.html

これは「ASに移行することで発生するリスク」より「移行しないリスク」が高くなったことを意味します。

今後、Android本体のバージョンやビルドツールなどのアップデートがあれば、ADTでアプリがビルドできなくなる可能性があります。
これまでADTで開発していたすべてのプロジェクトが開発を続けていくには、Android Studioに移行（Migrate）する必要があるのです。

とは言え、使い慣れてきたIDEを変えるのは抵抗があるものです。「以前、移行にチャレンジしたけど挫折した」と言う人も少なくはないでしょう。
また、ADTだけでなく、Antなどのビルドシステムで培ってきた資産をどのように移行すれば良いのか、不安に思う人もいることと思います。

そこで本書はまず、ADTのプロジェクトをAndroid Studioに移行について、準備から実際の手順を含めて解説し（@<chap>{adt2as}）、
次に、Antなどのビルドシステムで実現していた処理をAndroid Studio(Gradle)で実現するにはどのようにすればいいか、ユースケース毎に解説します（@<chap>{ant2gradle}）。

本書が、皆さんのAndroid Studio移行への道しるべとしての役割を果たせることを願っています。

==== ソフトウェアのバージョン
本書で取り上げるソフトウェアのバージョンは、次の通りです。

 * ADT
 ** Eclipse Mars 1. Release (4.5.1)
 ** ADT plugin 23.0.7
 * Android Studio 1.5.1
 * Android NDK r10e
