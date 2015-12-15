= ADTからAndroid Studioへの移行

//lead{
この章では、ADTのプロジェクト（ワークスペース）をAndroid Studioに移行する方法について解説します。
//}

== ADTプロジェクトのインポート機能
Android Studioには、ADTのプロジェクト（ワークスペース）を自動でインポートする機能があります。

//image[adt_import][Import Project(Eclipse ADT, Gradle etc.)][scale=0.30]{
//}

しかし、これはADTのワークスペースを「とりあえず使えるようにする」だけのもので、この機能を使った場合、Android Studio本来の性能を発揮できるプロジェクト構成にはなりません。

特に、ライブラリに依存していたり、複数のプロジェクトで構成されているワークスペースは、ビルドはおろか、インポートすら正常に完了しないということも起こります。

残念ながらADTそのものが開発とサポートが終了するソフトウェアなので、今後、この機能の拡充に開発リソースが注がれることは期待できないでしょう。

ADTからAndroid Studioへ移行する一番確実な方法は、Android Studioのプロジェクトを新しく作成して、ADTのワークスペースから各要素を手動で移行することだと筆者は考えます。

== 手動での移行（準備編）
それでは、実際にADTのワークスペースをAndroid Studioに移行していきましょう。

ここでは、ワークスペース@<tt>{farewelladt_workspace}を例に移行作業を進めます（@<list>{farewelladt_workspace}）。

//list[farewelladt_workspace][ADTのワークスペース構成]{
.
|-- FarewellAdt
|   |-- AndroidManifest.xml
|   |-- assets
|   |-- bin
|   |-- jni
|   |-- libs
|   |-- obj
|   |-- proguard-project.txt
|   |-- project.properties
|   |-- res
|   `-- src
`-- library
    |-- AndroidManifest.xml
    |-- assets
    |-- bin
    |-- libs
    |-- proguard-project.txt
    |-- project.properties
    |-- res
    `-- src
//}

=== 各プロジェクト設定を確認する
はじめに、現在のADTのワークスペースを構成する各プロジェクトの設定を確認します。

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
ユニットテストの利用	テストプロジェクト
NDKの利用	各プロジェクトのjniフォルダ
//}

例となるfarewelladt_workspaceには2つのプロジェクト@<tt>{FarewellAdt}と@<tt>{library}があります。
それぞれについて値を確認していきます。

//table[migration_checksheet][チェックシート記入例（FarewellAdtプロジェクト）]{
確認項目	確認する場所（ADT）	値
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
ユニットテストの利用	テストプロジェクト	なし
NDKの利用	各プロジェクトのjniフォルダ	あり
//}

//table[migration_checksheet][チェックシート記入例（libraryプロジェクト）]{
確認項目	確認する場所（ADT）	値
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
ユニットテストの利用	テストプロジェクト	なし
NDKの利用	各プロジェクトのjniフォルダ	なし
//}

===={adt_project_setting} プロジェクト設定
いくつかの設定は、ADTの画面上で確認する必要があります。

プロジェクトで右クリックして[Properties]をクリックすると、Project Propertiesが表示されます。
左側メニューから[Android]を選択すると@<img>{adt_project_setting}の画面になります。

//image[adt_project_setting][Project Build TargetとLibrary][scale=0.40]{
//}

確認が必要な情報は次の通りです。

 * [Project Build Target]で選択されているAndroidのバージョンのAPI Level
 * [Library] → [is library]のチェックの有無
 * [Reference]に登録されているプロジェクト名


== 手動での移行（実践編）
情報が揃ったら、いよいよ移行作業を始めます。
移行作業は次のステップで進めていきます。

 1. 新規プロジェクト（Studio）の作成
 2. モジュールの作成
 3. ライブラリの移行
 4. リソースの移行
 5. ソースコードの移行
 6. AndroidManifest.xmlの移行
 7. NDK(JNI)の移行

移行の途中でエラーが出ても慌てないでください。
エラーの修正は一通り移行作業が終わった後の方が負担が少ないので、まずは最後までやりきるように心がけましょう。

=== 新規プロジェクトの作成
移行先（Android Studio）でプロジェクトを作成します。
まずはアプリの起点となる@<tt>{FarewellAdt}から作成しましょう。

初期の画面から[Start a new Android Studio project]をクリックすると、プロジェクトウィザードが起動します。

==== Company domainとPackage name
[Application name]と[Company domain]をそれぞれ入力します。
これらを組み合わせたものがデフォルトの[Package name]になります。

//image[as_project_wizard1][][scale=0.30]{
//}

この際[Company domain]は自動的に逆順になります。
ADTのプロジェクトウィザードのように逆順で入力してしまうと、間違った[Package name]が設定されてしまうので注意が必要です。

なお、[Edit]をクリックすると、[Package name]を直接書き換えることができます。

==== 対応バージョン
対応する最低バージョンを選択します。ここで選択したAPI Levelが、minSdkVersionとして設定されます。
後から書き換えることもできますが、今回は先ほど確認した値「15」に対応するバージョン「Android 4.0.3 (Ice Cream Sandwich)」を選択します。

//image[as_project_wizard2][][scale=0.30]{
//}

最後に追加するActivityの選択画面です。今回はADTからの移行なのでActivityは必要ありません。

//image[as_project_wizard3][][scale=0.30]{
//}

[Add no Activity]を選択して[Finish]をクリックすると、Android Studioはプロジェクトの生成を開始します。

Android Studioでは、プロジェクトの生成時にはインターネット接続が必要となる場合があります。
プロジェクトの生成（ビルド）時に必要なソフトウェアをインターネットを通じてダウンロードするためです。


====[column] 管理の単位の違い

ADTとAndroid Studioでは「プロジェクト」という単語の意味が異なるので注意が必要です。

ADT（Eclipse）における大きな「まとまり」は「ワークスペース」と呼ばれ、ワークスペースの下には複数の「プロジェクト」が配置されます。
一方、Android Studio（IntelliJ IDEA）における大きな「まとまり」は「プロジェクト」であり、その下に「モジュール」が配置される構造になっています。

====[/column]

=== Project Viewへの切り替え
Android Studioは、プロジェクトの作成直後は標準で「Android View」が表示されます。
しかし「Android View」は、実際のプロジェクト構造が見えず、移行作業には適しません。

作業を進めるにあたってまず、左上のメニューから「Project View」に切り替えて下さい。

//image[project_view][［Project］を選択する。［Project Files］でないことに注意][scale=0.5]{
//}


=== モジュールの作成
次に、モジュールを作成します。
ADTでは「library」として扱っていたプロジェクトを「モジュール」にします。

[File]メニューから[New]→[New Module]をクリックすると、モジュール作成ウィザードが起動します。

まず、モジュールの種類を選択します。ADTの「library」プロジェクトの場合はライブラリなので、[Android Library]を選択します
（もしADTのプロジェクトがライブラリでなければ、[Phone and Tablet]など他の種類を選択します）。

//image[create_module1][][scale=0.30]{
//}

[Application/Library name]を入力@<fn>{capital}します。

//footnote[capital][［Application/Library name］は、先頭が大文字で入力することが推奨されていますが、ADTのプロジェクトに完全に合わせるなら小文字でも問題ありません]

次に、対応する最低バージョンを選択します。ここで選択したAPI Levelが、minSdkVersionとして設定されます。
ADTの「library」プロジェクトの値は「8」なので対応するバージョン「Android 2.2 (Froyo)」を選択します。

//image[create_module2][][scale=0.30]{
//}

[Finish]をクリックすると、Android Studioはモジュール「library」を生成します。

==== 参照（dependencies）の追加
ここまでを終えて、Android Studioのプロジェクトには「FarewellAdt」と「library」の２つのモジュールがあります。

しかし、この段階ではFarewellAdtはlibraryにあるコードやリソースを参照できません。
モジュール「FarewellAdt」から、モジュール「library」へ参照（dependencies）を設定する必要があります。

参照（dependencies）は、@<tt>{build.gradle}に記述して追加します。

モジュール「FarewellAdt」の@<tt>{app/build.gradle}を開いて、@<list>{add_dependencies}のように@<tt>{compile project(’:library’)}を追記します。

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

====[column] Gradle

Android Studioは、ビルドシステムに「Gradle」を採用しています。
「build.gradle」はGradleのビルド設定をまとめたファイル（ビルドファイル）です。

//image[gradlephant][Gradleのロゴ - Gradlephant][scale=0.75]{
//}

ADTでは、Eclipseとプラグインがビルドを実行して@<kw>{APK, Application PacKage}を生成しています。
また、@<kw>{CI, Continuous Integration}を実施しようとすれば、@<tt>{android}コマンドを使って生成できるAntのビルドファイルを使うのが一般的です。

しかし、ADTとAntはあくまで別のビルド環境です。
Antのビルド設定を複雑にしていくうちに、ADTではビルドできないプロジェクト構造になる（分断されてしまう）ことも珍しくありません。
それでは、ADTの統合開発環境としての機能がフルに発揮できなくなります。

一方、「Gradle」を使ってビルドするAndroid Studioは、ADTとAntのような分断が起こりません。

Gradleによるビルドは、ADTからAndroid Studioに移行する最も大きなメリットと言えるでしょう。

====[/column]

=== ライブラリの移行
続いて、ライブラリを移行します。

ADTのプロジェクト「FarewellAdt」と「library」は、どちらもライブラリ@<tt>{android-support-v4.jar}がlibsに配置されています。

Android Studioのモジュール「FarewellAdt」と「library」のbuild.gradleを開くと、どちらもappcompat-v7がdependenciesに追加されています
（@<list>{farewelladt_appcompat}）。

appcompat-v7は、android-support-v4を含みます@<fn>{compat-support}。
つまり、このケースではライブラリの移行は必要ないということになります。

//footnote[compat-support][Gradleでappcompat-v7を設定すると、自動的にandroid-support-v4の機能も取り込まれます]

//list[farewelladt_appcompat][appcompat-v7への参照（依存）が設定されている]{
dependencies {
    compile fileTree(dir: 'libs', include: ['*.jar'])
    ...
    compile 'com.android.support:appcompat-v7:23.1.1'
    ...
}
//}

==== リポジトリからライブラリを追加
Android Studio（Gradle）では基本的に、ライブラリの追加にあたってjarファイルをlibsに配置する必要がありません。
使いたいライブラリの名前とバージョンをdependenciesに記載すると、Gradleは、Maven Cnetral@<fn>{url_maven}やjCenter@<fn>{url_jcenter}などのリポジトリからバイナリを自動的にダウンロードしてビルドを実行します。

例えば、ButterKnifeを使う場合、ADTではButterKnifeのバイナリ（jar）ファイルをダウンロードしてlibsフォルダにコピーします。
一方、Android Studio（Gradle）では@<list>{add_butterknife}のようにdependenciesにButterKnifeの情報を記載するだけで設定が完了します。

もちろん、リポジトリに登録されていないライブラリについては、これまで通りjarファイルをlibsに置くことができます。

//footnote[url_maven][Maven Central: http://search.maven.org/]
//footnote[url_jcenter][jCenter: https://bintray.com/bintray/jcenter]

//list[add_butterknife][ButterKnifeをdependenciesに追加]{
dependencies {
    compile fileTree(dir: 'libs', include: ['*.jar'])
    ...
    compile 'com.jakewharton:butterknife:7.0.1'
}
//}

=== リソースの移行
続いて、プロジェクトのリソースを移行しましょう。

ADTのプロジェクトのリソースは、@<tt>{res}ディレクトリ以下にあります。
Android Studioの各モジュールの@<tt>{src/main/res}ディレクトリ以下にコピーします。

====[column] drawableとmipmap

ADTでは、アプリのアイコンは標準で@<tt>{res/drawable-*}に配置されました。

一方、Android Studioでは、プロジェクト作成時点で@<tt>{src/main/res/mipmap-*}に配置されます。

mipmap@<fn>{about_mipmap}は、3Dテクスチャとしての描画に最適化された画像群の名称ですが、実際にはこれまで@<tt>{res/drawable-*}に置いていた画像と違いはありません。
PNG形式で、各解像度別の画像サイズも同じです。

しかし、@<tt>{src/main/res/mipmap-*}に配置したファイルは@<tt>{R.mipmap.*}や@<tt>{@mipmap/*}を通じてアクセスすることに注意して下さい。
ソースコードやリソースから@<tt>{drawable}として参照していた画像を@<tt>{mipmap}に置く場合、それぞれの参照を変更しなくてはなりません。

ADTから画像リソースを移行する場合、アイコン画像は@<tt>{src/main/res/mipmap-*}に移動する。
また@<tt>{src/main/AndroidManifest.xml}を開いて、アイコンの参照先をADTの時に設定した@<tt>{@drawable/ic_launcher}から@<tt>{@mipmap/ic_launcher}に変更する必要するなどして調整してください。

//footnote[about_mipmap][Wikipedia mipmap:@<br>{}https://ja.wikipedia.org/wiki/%E3%83%9F%E3%83%83%E3%83%97%E3%83%9E%E3%83%83%E3%83%97]

====[/column]

==== assets
ADTのプロジェクトのアセットは、@<tt>{assets}ディレクトリ以下にあります。
Android Studioの各モジュールの@<tt>{src/main/assets}ディレクトリ以下にコピーします。

=== Javaソースコードの移行
ADTのプロジェクトのJavaソースコードは、@<tt>{src}ディレクトリ以下にあります。
Android Studioの各モジュールの@<tt>{src/main/java}ディレクトリ以下にコピーします。

この際、ADTからパッケージを選択してコピー&ペーストすると、パッケージ構造を正確にコピーできない場合があります。
エクスプローラー（Macの場合はFinder）などを使ってディレクトリ（パッケージ）ごとコピーしてください。

=== テストコードの移行
ADTにテストコードがある場合、Android Studioの各モジュールの@<tt>{src/AndroidTest/java}ディレクトリ以下にコピーします
（Android Studioは、テストコードをモジュール毎に管理します）。

この際、ADTからパッケージを選択してコピー&ペーストすると、パッケージ構造を正確にコピーできない場合があります。
エクスプローラー（Macの場合はFinder）などを使ってディレクトリ（パッケージ）ごとコピーしてください。

=== AndroidManifest.xmlの移行
最後に@<tt>{AndroidManifest.xml}をコピーします。

ADTのプロジェクトのAndroidManifest.xmlは、プロジェクトの直下にあります。
Android Studioの各モジュールの@<tt>{src/main/}ディレクトリにコピーします。

==== build.gradleへ項目を移動する
ADTでAndroidManifest.xmlに設定していた値のいくつかは、Android Studioではbuild.gradleに設定するように変更されています。

build.gradleへ移動する項目を@<table>{move_to_buildgradle}に示します。

//table[move_to_buildgradle][build.gradleへ移動する項目]{
作業項目
---------------
compileSdkVersion
minSdkVersion
targetSdkVersion
versionCode
versionName
//}

これらの値をAndroidManifest.xml（@<list>{adt_android_manifest}）からbuild.gradleに移動します。

//list[adt_android_manifest][]{
<?xml version="1.0" encoding="utf-8"?>
<manifest xmlns:android="http://schemas.android.com/apk/res/android"
    package="io.keiji.farewelladt"
    android:versionCode="1"
    android:versionName="1.0" >

    <uses-sdk
        android:minSdkVersion="15"
        android:targetSdkVersion="23" />

    <application
        android:allowBackup="true"
        android:icon="@drawable/ic_launcher"
        android:label="@string/app_name"
        android:theme="@style/AppTheme" >
        <activity
            android:name=".MainActivity"
            android:label="@string/app_name" >
            <intent-filter>
                <action android:name="android.intent.action.MAIN" />

                <category android:name="android.intent.category.LAUNCHER" />
            </intent-filter>
        </activity>
    </application>

</manifest>
//}

次に、［versionCode］と［versionName］の値を@<tt>{build.gralde}に移行します（@<list>{as_buildgradle}）。

//list[as_buildgradle][]{
apply plugin: 'com.android.application'

android {
    compileSdkVersion 23
    buildToolsVersion "23.0.2"

    defaultConfig {
        applicationId "io.keiji.farewelladt"
        minSdkVersion 15
        targetSdkVersion 23
        versionCode 1
        versionName "1.0"
    }
    buildTypes {
        release {
            minifyEnabled false
            proguardFiles getDefaultProguardFile('proguard-android.txt'), 'proguard-rules.pro'
        }
    }
}

dependencies {
    compile fileTree(include: ['*.jar'], dir: 'libs')
    testCompile 'junit:junit:4.12'
    compile 'com.android.support:appcompat-v7:23.1.1'
    compile project(':library')
}
//}

最後に、移行した値をAndroid StudioにコピーしたAndroidManifest.xmlから削除すれば移行は完了です。

=== NDKの移行
@<kw>{NDK,Native Development Kit}を使っている場合は、ネイティブのコードを移行するのに加え、NDKを扱えるようにGradleを設定する必要があります。

==== NDKのパスを設定
Android StudioにNDKのパスを設定します。
プロジェクトで右クリックをして［Open Module Settings］をクリックすると設定画面が開きます。

［SDK Locations］の一番下の@<tt>{Android NDK Location}に、NDKのパスを設定します。

//image[ndk_setting][Android NDK Location][scale=0.30]{
//}

==== ネイティブのソースコード
ADTのプロジェクトのネイティブのソースコードは、@<tt>{jni}ディレクトリにあります。
Android Studioの各モジュールの@<tt>{src/main/jni}ディレクトリにコピーします。

==== build.gradleの設定
続いて、build.gradleをNDK用に設定します。

まず、プロジェクトのトップレベルにあるbuild.gradleを変更します（@<list>{top_build_gradle}）。
本稿執筆時点で、NDKビルドに対応しているのは実験（experimental）バージョンのGradleだけです。

//list[top_build_gradle][プロジェクトのトップにあるbuild.gradle]{
// Top-level build file where you can add configuration options common to all sub-projects/modules.

buildscript {
    repositories {
        jcenter()
    }
    dependencies {
        classpath "com.android.tools.build:gradle-experimental:0.4.0"
        // NOTE: Do not place your application dependencies here; they belong
        // in the individual module build.gradle files
    }
}

allprojects {
    repositories {
        jcenter()
    }
}

task clean(type: Delete) {
    delete rootProject.buildDir
}
//}

なお、本稿執筆時点（2015年12月）で最新のgradle-experimentalのバージョンは@<tt>{0.6.0-alpha2}です。
@<tt>{0.6.0-alpha2}に設定した場合、@<tt>{gradle/wrapper/gradle-wrapper.properties}で@<tt>{gradle-2.9-all.zip}を指定する必要があります。

次に、プロジェクト「FarewellAdt」と「library」それぞれに適用するpluginを変更します。

@<list>{farewelladt_buildgradle}は、@<tt>{com.android.application}を@<tt>{com.android.model.application}に変更しています。

@<tt>{com.android.model.application}は通常のプラグインと@<kw>{DSL,Domain-Specific Language}が違うので注意して下さい。

@<tt>{android}は@<tt>{model}の下に位置します。
@<tt>{compileSdkVersion}などの値も空白ではなく@<tt>{=}で指定し、@<tt>{defaultConfig}には@<tt>{.with}が必要です。
また、API Levelの指定は@<tt>{minSdkVersion.apiLevel}のようになります。

//list[farewelladt_buildgradle][NDKのDSL用に記述を調整]{
apply plugin: 'com.android.model.application'

model {
    android {
        compileSdkVersion = 23
        buildToolsVersion = "23.0.2"

        defaultConfig.with {
            applicationId = "io.keiji.farewelladt"
            minSdkVersion.apiLevel = 15
            targetSdkVersion.apiLevel = 23
            versionCode = 1
            versionName = "1.0"
        }
    }

    android.buildTypes {
        release {
            minifyEnabled = false
            proguardFiles.add(file("proguard-rules.pro"))
        }
    }

    android.ndk {
        moduleName = "hello-jni"
    }
}

dependencies {
    compile fileTree(include: ['*.jar'], dir: 'libs')
    compile 'com.android.support:appcompat-v7:23.1.1'
}
//}

プロジェクト「library」のプラグインも@<tt>{com.android.model.library}に変更します（@<list>{library_buildgradle}）。

//list[library_buildgradle][]{
apply plugin: 'com.android.model.library'

model {
    android {
        compileSdkVersion = 23
        buildToolsVersion = "23.0.2"

        defaultConfig.with {
            minSdkVersion.apiLevel = 15
            targetSdkVersion.apiLevel = 23
        }
    }

    android.buildTypes {
        release {
            minifyEnabled = false
            proguardFiles.add(file("proguard-rules.pro"))
        }
    }
}

dependencies {
    compile fileTree(dir: 'libs', include: ['*.jar'])
    compile 'com.android.support:appcompat-v7:23.1.1'

}
//}

変更点は@<tt>{com.android.model.application}を指定した場合と共通ですが、@<tt>{versionCode}と@<tt>{versionName}を削除するのを忘れないようにして下さい。

ライブラリの場合、@<tt>{defaultConfig.with}の中に@<tt>{versionCode}や@<tt>{versionName}があると、@<tt>{org.gradle.api.internal.ExtensibleDynamicObject}が原因でビルドに失敗します。

NDKに関する最新の情報、詳細な設定については次のサイトを参照して下さい。

 * 参考
 ** http://tools.android.com/tech-docs/new-build-system/gradle-experimental
