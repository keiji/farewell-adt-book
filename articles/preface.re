= はじめに
本書は、ADTからAndroid Studioに移行する人たちの「道しるべ」になることを目的に執筆しました。

== ADTとは
@<kw>{ADT,Android Developer Tools}は、2007年にAndroidが発表されて以来、普及してきたAndroidアプリの@<kw>{IDE, 統合開発環境}です。

//image[adt_icon][単体版のADTには、Eclipseにプラグインをインストールした場合と異なるアイコンが設定されていた][scale=0.15]{
//}

初期はEclipseに追加するプラグインの形態で配布されていましたが、中期以降は、最初からEclipseに組み込まれた状態で配布されるようになりました。

== Android Studioとは
Android Studioは、2013年のGoogle I/Oで発表された統合開発環境です。

//image[android_studio_icon][Android Studioのアイコンはマテリアルデザインに則っている][scale=0.12]{
//}

EclipseをベースにしたADTに対して、Android StudioのベースはチェコJetBRAINS社のIntelli J IDEAのオープンソース版（Community Edition）です。
Intelli J IDEAの持つ強力な補完機能に加えて、ADTには無かった機能も新しく開発・追加されています。

== Time to Migrate
2013年にAndroid Studioのプレビュー版が発表されてからも、ADTは公式のIDEとして配布されていました。
しかし、2015年にAndroid Studioが1.0になると、「公式」の冠はADTからAndroid Studioに移され、再びADTはプラグインのみの提供となりました。

さらにGoogle社は、2015年末でADTの開発やサポートを打ち切ることをアナウンスしています。

 * An update on Eclipse Android Developer Tools
 ** http://android-developers.blogspot.jp/2015/06/an-update-on-eclipse-android-developer.html

サポートがなくなれば、今後、Android本体のバージョンやビルドツールなどのアップデートがあれば、ADTでアプリがビルドできなくなる可能性があります。

これは「ASに移行することで発生するリスク」より「移行しないリスク」が高くなったことを意味します。

すべてのAndroidアプリ開発のプロジェクトがこれからも開発を続けていくには、Android Studioに移行（Migrate）する必要があるのです。

とは言え、これまで使い慣れてきたIDEを変えるのには抵抗があるものです。
また、ADTだけでなく、Antなどのビルドシステムを使って工夫してきた資産をどのように移行すれば良いのか、不安に思う人もいるでしょう。

本書の内容が、そんな皆さんの道しるべになることを願っています。

== 免責事項
本書に記載された内容は、情報の提供のみを目的としています。したがって、本書を用いた開発、製作、運用は、必ずご自身の責任と判断によって行ってください。
これらの情報による開発、製作、運用の結果について、著者はいかなる責任も負いません。

== 表記関係について
本書に記載されている会社名、製品名などは、一般に各社の登録商標または商標、商品名です。
会社名、製品名については、本文中では©、®、™マークなどは表示していません。

== 著作権について
本書は、有山圭二の著作物です。また、本書の表紙及び裏表紙のイラストは根雪れいの著作物です。
本書の全部、または一部について、著作者から文書による許諾を得ずに複製することは禁じられています。
