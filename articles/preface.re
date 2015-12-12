= はじめに

このたびはAndroid Studio完全移行ガイドを手にとっていただき、ありがとうございます。

本書が題材にしている「Android Studio」は、2013年のGoogle I/Oで発表された@<kw>{IDE, 統合開発環境}です。

//image[android_studio_icon][Android Studioのアイコンはマテリアルデザインに則っている][scale=0.08]{
//}

チェコJetBRAINS社が開発するIntelli J IDEAのオープンソース版（Community Edition）をベースに開発されています。

一方、@<kw>{ADT,Android Developer Tools}は、2007年にAndroidが発表されて以来、普及してきた統合開発環境です。

//image[adt_icon][単体版のADTのアイコン][scale=0.1]{
//}

初期はEclipseに追加するプラグインの形態で配布されていましたが、中期以降は、最初からEclipseに組み込まれた状態（単体版）で配布されるようになりました。

2015年1月、Android Studioのバージョンが1.0になると、「公式開発環境」の冠はADTからAndroid Studioに移され、ADTは再びプラグインのみの提供となりました。

さらにGoogle社は、2015年12月末でADTの開発やサポートを打ち切ることをアナウンスしています。

 * An update on Eclipse Android Developer Tools
 ** http://android-developers.blogspot.jp/2015/06/an-update-on-eclipse-android-developer.html

サポートがなくなれば、今後、Android本体のバージョンやビルドツールなどのアップデートがあれば、ADTでアプリがビルドできなくなる可能性があります。
これは「ASに移行することで発生するリスク」より「移行しないリスク」が高くなったことを意味します。

ADTの開発サポート終了に伴い、これまでADTで開発していたすべてのプロジェクトが開発を続けていくには、Android Studioに移行（Migrate）する必要があります。

とは言え、これまで使い慣れてきたIDEを変えるのには抵抗があるものです。
また、ADTだけでなく、Antなどのビルドシステムを使って工夫してきた資産をどのように移行すれば良いのか、不安に思う人もいるでしょう。

そこで本書はまず、ADTのプロジェクトをAndroid Studioに移行について、準備から実際の手順を含めて解説します（@<chap>{adt2as}）。

次に、Antなどのビルドシステムで実現していた処理をAndroid Studio(Gradle)で実現するにはどのようにすればいいか、ユースケース毎に解説します（@<chap>{ant2gradle}）。

本書が、皆さんのAndroid Studio移行への道しるべとしての役割を果たせることを願っています。
