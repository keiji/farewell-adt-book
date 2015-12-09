= はじめに

このたびはAndroid Studio完全移行ガイドを手にとっていただき、ありがとうございます。

本書は、「ADT」から「Android Studio」に移行する人たちの「道しるべ」になることを目的に執筆しました。

== ADTとは
@<kw>{ADT,Android Developer Tools}は、2007年にAndroidが発表されて以来、普及してきたAndroidアプリの@<kw>{IDE, 統合開発環境}です。

//image[adt_icon][単体版のADTには、Eclipseにプラグインをインストールした場合と異なるアイコンが設定されていた][scale=0.15]{
//}

初期はEclipseに追加するプラグインの形態で配布されていましたが、中期以降は、最初からEclipseに組み込まれた状態で配布されるようになりました。

== Android Studioとは
Android Studioは、2013年のGoogle I/Oで発表された統合開発環境です。

//image[android_studio_icon][Android Studioのアイコンはマテリアルデザインに則っている][scale=0.12]{
//}

EclipseをベースにしたADTに対して、Android StudioはチェコJetBRAINS社が開発するIntelli J IDEAのオープンソース版（Community Edition）をベースにしています。
Intelli J IDEAの持つ強力な補完機能に加えて、ADTには無かった機能も新しく開発・追加されています。

== Time to Migrate
2013年にAndroid Studio（プレビュー版）が発表されてからも、ADTは公式のIDEとして配布されていました。
しかし、2015年にAndroid Studioが1.0になると、「公式」の冠はADTからAndroid Studioに移され、再びADTはプラグインのみの提供となりました。

さらにGoogle社は、2015年末でADTの開発やサポートを打ち切ることをアナウンスしています。

 * An update on Eclipse Android Developer Tools
 ** http://android-developers.blogspot.jp/2015/06/an-update-on-eclipse-android-developer.html

サポートがなくなれば、今後、Android本体のバージョンやビルドツールなどのアップデートがあれば、ADTでアプリがビルドできなくなる可能性があります。

これは「ASに移行することで発生するリスク」より「移行しないリスク」が高くなったことを意味します。

これまでADTで開発していたすべてのプロジェクトが、これからも開発を続けていくには、Android Studioに移行（Migrate）する必要があるのです。

とは言え、これまで使い慣れてきたIDEを変えるのには抵抗があるものです。
また、ADTだけでなく、Antなどのビルドシステムを使って工夫してきた資産をどのように移行すれば良いのか、不安に思う人もいるでしょう。

本書はまず、ADTのプロジェクトをAndroid Studioに移行する手順について解説します。

次に、Antなどのビルドシステムで実現していた処理をAndroid Studio(Gradle)で実現するにはどのようにすればいいか、ユースケース毎に解説します。

本書の内容が、そんな皆さんの道しるべになることを願っています。
