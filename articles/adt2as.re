= ADTからAndroid Studioへの移行

////lead{
この章では、ADTのプロジェクト（ワークスペース）をAndroid Studioに移行する方法について解説します。
//}

== ADTプロジェクトのインポート機能
Android Studioには、ADTのプロジェクト（ワークスペース）を自動でインポートする機能があります。

//image[adt_import][Import Project(Eclipse ADT, Gradle etc.)][scale=0.25]{
//}

しかしこの機能は、ADTのワークスペースを「とりあえず使えるようにする」だけのもので、この機能を使った場合、Android Studio本来の性能を発揮できるプロジェクト構成にはなりません。

特に、ライブラリに依存していたり、複数のプロジェクトで構成されているワークスペースは、ビルドはおろか、インポートすら完了しないということも起こります。

残念ながらADTそのものが開発とサポートが終了するソフトウェアなので、今後、この機能の拡充に開発リソースが注がれることは期待できません。

一番確実な方法は、Android Studioのプロジェクト構造を理解した上で、ADTのワークスペースを手動で移行することです。

== 手動での移行（準備編）

それでは、実際にADTのワークスペースをAndroid Studioに移行していきます。

ここでは、ワークスペース@<tt>{farewelladt_workspace}を例に移行作業を進めます。

=== 各プロジェクト設定を確認する
はじめに現在のADTのワークスペースを構成する各プロジェクトの設定を確認します。

確認する内容を@<table>{migration_checksheet}に示します。

//table[migration_checksheet][プロジェクトチェックシート]{
確認項目	確認する場所（ADT）	値
---------------
プロジェクト名	ADTのプロジェクト画面	　　　　　　　　　　　　　　　
パッケージ名	AndroidManifest.xml（package）
バージョンコード	AndroidManifest.xml（versionCode）
バージョン名	AndroidManifest.xml（versionName）
対応API Level	AndroidManifest.xml（minSdkVersion）
対象API Level	AndroidManifest.xml（targetSdkVersion）
Project Build Target	@<hd>{adt_project_setting}参照
ライブラリプロジェクト	@<hd>{adt_project_setting}参照
依存するプロジェクト	@<hd>{adt_project_setting}参照
利用ライブラリ	各プロジェクトのlibsフォルダ
NDKの利用	各プロジェクトのjniフォルダ
//}

例となるfarewelladt_workspaceには2つのプロジェクト@<tt>{FarewellAdt}と@<tt>{library}があります。
それぞれについて値を確認していきます。

//table[migration_checksheet][チェックシート記入例（FarewellAdtプロジェクト）]{
確認項目	確認する場所（ADT）	値の例
---------------
プロジェクト名	ADTのプロジェクト画面	FarewellAdt
パッケージ名	AndroidManifest.xml（package）	io.keiji.farewelladt
バージョンコード	AndroidManifest.xml（versionCode）	1
バージョン名	AndroidManifest.xml（versionName）	1.0
対応API Level	AndroidManifest.xml（minSdkVersion）	15
対象API Level	AndroidManifest.xml（targetSdkVersion）	23
Project Build Target	@<hd>{adt_project_setting}参照	Android 6.0 - API Level 23
ライブラリプロジェクト	@<hd>{adt_project_setting}参照	false
依存するプロジェクト	@<hd>{adt_project_setting}参照	library
利用ライブラリ	各プロジェクトのlibsフォルダ	android-support-v4.jar
NDKの利用	各プロジェクトのjniフォルダ	あり
//}

//table[migration_checksheet][チェックシート記入例（libraryプロジェクト）]{
確認項目	確認する場所（ADT）	値の例
---------------
プロジェクト名	ADTのプロジェクト画面	library
パッケージ名	AndroidManifest.xml（package）	io.keiji.library
バージョンコード	AndroidManifest.xml（versionCode）	1
バージョン名	AndroidManifest.xml（versionName）	1.0
対応API Level	AndroidManifest.xml（minSdkVersion）	8
対象API Level	AndroidManifest.xml（targetSdkVersion）	21
Project Build Target	@<hd>{adt_project_setting}参照	Android 6.0 - API Level 23
ライブラリプロジェクト	@<hd>{adt_project_setting}参照	true
依存するプロジェクト	@<hd>{adt_project_setting}参照	なし
利用ライブラリ	各プロジェクトのlibsフォルダ	android-support-v4.jar
NDKの利用	各プロジェクトのjniフォルダ	なし
//}

===={adt_project_setting} プロジェクト設定
いくつかの設定はファイルではなく、ADTの画面上で確認する必要があります。

プロジェクトで右クリックして[Properties]をクリックすると、Project Propertiesが表示されます。
左側メニューから[Android]を選択すると@<img>{adt_project_setting}の画面になります。

//image[adt_project_setting][Project Build TargetとLibrary][scale=0.25]{
//}

必要な情報は、次の通りです。

 * [Project Build Target]で選択されているAndroidのバージョンのAPI Level
 * [Library] → [is library]のチェックの有無
 * [Reference]に登録されているプロジェクト名


== 手動での移行（実践編）
必要な情報が揃ったら、いよいよ移行作業を始めます。
移行作業は次のステップで進めていきます。

 1. プロジェクト（Studio）の作成
 2. モジュールの作成
 3. ライブラリの移行
 4. AndroidManifest.xmlの移行
 5. リソースの移行
 6. NDK(JNI)の移行
 7. ソースコードの移行

移行の途中でエラーが出ても慌てないでください。
エラーの修正は一通り移行作業が終わった後の方が負担が少ないので、まずは最後までやりきるように心がけましょう。

=== 新規プロジェクトの作成
移行先（Android Studio）でプロジェクトを作成します。
まずはアプリの起点となる@<tt>{FarewellAdt}から作成しましょう。

初期の画面から[Start a new Android Studio project]をクリックすると、プロジェクトウィザードが起動します。

==== Company domainとPackage name
[Application name]と[Company domain]をそれぞれ入力します。
これらを組み合わせたものがデフォルトの[Package name]になります。

//image[as_project_wizard1][][scale=0.2]{
//}

この際[Company domain]は自動的に逆順になります。
ADTのプロジェクトウィザードのように逆順で入力してしまうと、間違った[Package name]が設定されてしまうので注意が必要です。

なお、[Edit]をクリックすると、[Package name]を直接書き換えることができます。

==== 対応バージョン
対応する最低バージョンを選択します。ここで選択したAPI Levelが、minSdkVersionとして設定されます。
後から書き換えることもできますが、先ほど確認した値「15」に対応するバージョン「Android 4.0.3 (Ice Cream Sandwich)」を選択します。

//image[as_project_wizard2][][scale=0.2]{
//}

最後に追加するActivityの選択画面です。今回はADTからの移行なのでActivityは必要ありません。

//image[as_project_wizard3][][scale=0.2]{
//}

[Add no Activity]を選択して[Finish]をクリックすると、Android Studioはプロジェクトの生成を開始します。

Android Studioでは、プロジェクトの生成時にはインターネット接続が必要となる場合があります。
プロジェクトの生成（ビルド）時に必要なソフトウェアをインターネットを通じてダウンロードするためです。


====[column] 管理の単位の違い

ADTとAndroid Studioでは「プロジェクト」という単語の意味が異なるので注意が必要です。

ADT（Eclipse）における大きな「まとまり」は「ワークスペース」と呼ばれ、ワークスペースの下には複数の「プロジェクト」が配置されます。
一方、Android Studio（IntelliJ IDEA）における大きな「まとまり」は「プロジェクト」であり、その下に「モジュール」が配置される構造になっています。

====[/column]

=== モジュールの作成
次に、モジュールを作成します。
ADTでは「library」として扱っていたプロジェクトをモジュールにします。

[File]メニューから[New]→[New Module]をクリックすると、モジュール作成ウィザードが起動します。

まず、モジュールの種類を選択します。ここで追加したいADTの「library」プロジェクトはライブラリなので、[Android Library]を選択します
（もしADTのプロジェクトがライブラリでなければ、[Phone and Tablet]など他の種類を選択します）。

//image[create_module1][][scale=0.2]{
//}

[Application/Library name]を入力@<fn>{capital}します。

//footnote[capital][［Application/Library name］は、先頭が大文字で入力することが推奨されていますが、ADTのプロジェクトに完全に合わせるなら小文字でも問題ありません]

次に、対応する最低バージョンを選択します。ここで選択したAPI Levelが、minSdkVersionとして設定されます。
ADTの「library」プロジェクトの値は「8」なので対応するバージョン「Android 2.2 (Froyo)」を選択します。

//image[create_module2][][scale=0.2]{
//}

[Finish]をクリックすると、Android Studioはモジュール「library」を生成します。

==== 参照（dependencies）の追加
ここまでで、Android Studioのプロジェクトには「FarewellAdt」と「library」の２つのモジュールがあります。

しかし、この段階ではモジュール「FarewellAdt」は、モジュール「library」への参照（dependencies）を設定していないので、FarewellAdtはlibraryにあるコードやリソースを参照できません。

参照（dependencies）は、@<tt>{build.gradle}に記述して追加します。

モジュール「FarewellAdt」の@<tt>{app/build.gradle}@<fn>{androidview}を開いて、@<list>{add_dependencies}のように@<tt>{compile project(’:library’)}を追記します。

//list[add_dependencies][libraryへの参照（dependencies）を設定]{
apply plugin: 'com.android.application'

android {
    ...
}

dependencies {
    ...
    compile project(':library')
}
//}

//footnote[androidview][［Project View］を表示している場合の場所です。［Android View］の場合は@<tt>{Gradle Scripts}の@<tt>{build.gradle(Module: app)}になります]

====[column] Gradle

Android Studioは、ビルドシステムに「Gradle」を採用しています。
「build.gradle」はGradleのビルド設定をまとめたファイル（ビルドファイル）です。

//image[gradlephant][Gradleのロゴ - Gradlephant][scale=0.5]{
//}

ADTでは、Eclipseとプラグインが@<kw>{APK, Aplication PacKage}をビルドしていました。
@<kw>{CI, Continuous Integration}を実施しようとすれば、@<tt>{android}コマンドを使って生成できるAntのビルドファイルを使うしか方法がありませんでした。

しかし、ADTとAntはあくまで別のビルド環境です。
Antのビルド設定を複雑にしていくうちに、ADTではビルドできないプロジェクト構造になるということも珍しくありませんでした。
それでは、ADTの統合開発環境としての機能がフルに発揮できなくなります。

一方、Android Studioは元々「Gradle」を使ってビルドするので、ADTとAntのような分断が起こりません。
また、ビルドファイルをカスタムすることで様々な用途でビルドすることができます。

Gradleによるビルドは、ADTからAndroid Studioに移行する最も大きなメリットと言えるでしょう。

====[/column]

=== ライブラリの移行
続いて、ライブラリを移行します。

ADTのプロジェクト「FarewellAdt」と「library」は、どちらも@<tt>{android-support-v4.jar}をライブラリとしてlibsに持っています。

Android Studioのモジュール「FarewellAdt」と「library」のbuild.gradleを開くと、どちらもappcompat-v7がdependenciesに追加されています
（@<list>{farewelladt_appcompat}および@<list>{library_appcompat}）。

appcompat-v7は、android-support-v4を含みます。
つまり、このケースではライブラリの移行は必要ないということになります。

//list[farewelladt_appcompat][appcompat-v7への参照（依存）が設定されている]{
dependencies {
    compile fileTree(dir: 'libs', include: ['*.jar'])
    ...
    compile 'com.android.support:appcompat-v7:23.1.1'
    ...
}
//}

//list[library_appcompat][appcompat-v7への参照（依存）が設定されている]{
dependencies {
    compile fileTree(dir: 'libs', include: ['*.jar'])
    ...
    compile 'com.android.support:appcompat-v7:23.1.1'
}
//}

ここで重要なのは、Android Studio（Gradle）では基本的に、ライブラリの追加をjarファイルで行わなくても良いということです。
使いたいライブラリの名前とバージョンをdependenciesに記載しておくだけで、Gradleは、Maven Cnetral@<fn>{url_maven}やjCenter@<fn>{url_jcenter}などのリポジトリからバイナリを自動的にダウンロードしてビルドを実行します。

例えば、ButterKnifeを使いたい場合、ADTではButterKnifeのバイナリ（jar）ファイルをダウンロードしてlibsフォルダにコピーします。
一方、Android Studio（Gradle）では@<list>{add_butterknife}のようにdependenciesにButterKnifeの情報を記載するだけです。

一方、リポジトリに登録されていないライブラリについては、これまで通りjarファイルをlibsに置きます。

//footnote[url_maven][Maven Central: http://search.maven.org/]
//footnote[url_jcenter][jCenter: https://bintray.com/bintray/jcenter]

//list[add_butterknife][ButterKnifeをdependenciesに追加]{
dependencies {
    compile fileTree(dir: 'libs', include: ['*.jar'])
    ...
    compile 'com.jakewharton:butterknife:7.0.1'
}
//}

=== AndroidManifest.xml

==== build.gradleへ移動する項目

//table[move_to_buildgradle][build.gradleへ移動する項目]{
作業項目
---------------
compileSdkVersion
minSdkVersion
targetSdkVersion
versionCode
versionName
//}

=== Javaソースコード

=== リソース

==== mipmap

==== assets

=== 依存関係（dependencies）の設定

=== Proguard設定

=== NDKの移行

==== NDKのパスを設定

//image[ndk_setting][Android NDK Location][scale=0.2]{
//}

==== ソースコードを移動

==== build.gradleの設定

=== ADTとAndroid Studioのプロジェクト構造の違い

==== ワークスペース

==== プロジェクト
