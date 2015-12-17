= はじめに

「Android Studio完全移行ガイド」を手にとっていただき、ありがとうございます。

はじめに、本書が題材にしている「Android Studio」と「ADT」について解説します。

//image[as_adt_icons][Android Studio（左）とADT（右）][scale=0.12]{
//}

「Android Studio」は、Google I/O 2013で発表された、Androidアプリ開発の@<kw>{IDE, 統合開発環境}です。
チェコJetBrains社が開発している「IntelliJ IDEA@<fn>{intellij}」をベースに開発されています。

一方、@<kw>{ADT,Android Developer Tools}は、2007年にAndroidが発表されて以来、普及してきたEclipseベースのIDEです。
当初はEclipseのプラグインとして配布され、中期以降はEclipseに組み込まれて単体版が配布されていました。

//footnote[intellij][「IntelliJ IDEA Community Edition」をベースに開発されています]

== Time to Migrate
2015年1月、Android Studioのバージョンが1.0になると同時に「公式開発環境」の冠はAndroid Studioに移され、ADTは再びプラグインによる提供のみとなりました。

更に米Google社は、それから半年経つか経たないかのうちに、2015年末でADTの開発やサポートを打ち切ることをアナウンスしました。

 * An update on Eclipse Android Developer Tools
 ** http://android-developers.blogspot.jp/2015/06/an-update-on-eclipse-android-developer.html

これは「ASに移行することで発生するリスク」より「移行しないリスク」が高くなったことを意味します。
今後、Android本体のバージョンやビルドツールなどのアップデートがあれば、ADTでアプリがビルドできなくなる可能性があるからです。

これまでADTで開発していたすべてのプロジェクトが開発を続けていくには、Android Studioに移行（Migrate）する必要があります。

とは言え、使い慣れてきたIDEを変えるのは抵抗があるものです。「移行にチャレンジしたけど挫折した」と言う人も少なくはないでしょう。
また、ADTだけでなく、Antなどのビルドシステムで培ってきた資産をどのように移行すれば良いのか、不安に思う人もいることと思います。

本書ではまず、ADTのプロジェクトからAndroid Studioへの移行について、準備から実際の手順を含めて解説します（@<chap>{adt2as}）。
次に、Antなどのビルドシステムで実現していた処理をAndroid Studio(Gradle)で実現するにはどのようにすればいいか、ユースケース毎に解説します（@<chap>{ant2gradle}）。

本書が、皆さんのAndroid Studioへの道しるべになることを願っています。

== ソフトウェアのバージョン
本書で取り上げるソフトウェアのバージョンは、次の通りです。

 * ADT
 ** Eclipse Mars 1. Release (4.5.1)
 ** ADT plugin 23.0.7
 * Android Studio 1.5.1
 * Android NDK r10e
