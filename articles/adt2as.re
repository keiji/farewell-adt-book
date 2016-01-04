= Android Studioへの移行

//lead{
この章では、ADTのプロジェクト（ワークスペース）をAndroid Studioに移行する方法について解説します。
//}

== ADTプロジェクトのインポート機能について
Android Studioには、ADTのプロジェクト（ワークスペース）を自動でインポートする機能があります。

//image[adt_import][Import Project(Eclipse ADT, Gradle etc.)][scale=0.35]{
//}

しかし、これはADTのワークスペースを「とりあえず使えるようにする」だけのものです。
この機能を使った場合、Android Studio本来の性能を発揮できるプロジェクト構成にはなりません。

特に、ライブラリに依存していたり、複数のプロジェクトで構成されているワークスペースは、インポートすら正常に完了しないということも起こります。

残念ながらADTそのものが開発とサポートが終了するソフトウェアなので、今後、この機能の拡充に開発リソースが注がれることは期待できません。

ADTからAndroid Studioへ移行する一番確実な方法は、Android Studioのプロジェクトを新しく作成して、ADTのワークスペースから各要素を手動で移行することだと筆者は考えます。

== 手動での移行（準備編）
それでは、実際にADTのワークスペースをAndroid Studioに移行していきましょう。

ここでは、ワークスペース「farewelladt_workspace」を例に移行作業を進めます（@<list>{farewelladt_workspace}）。

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
確認項目	確認する場所（ADT）
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
ユニットテスト	テストプロジェクト
NDK	各プロジェクトのjniフォルダ
//}

===={adt_project_setting} プロジェクト設定
いくつかの設定は、ADTの画面上で確認する必要があります。

プロジェクト一覧で右クリックして［Properties］をクリックすると、Project Propertiesが表示されます。
左側メニューから［Android］を選択すると@<img>{adt_project_setting}の画面になります。

//image[adt_project_setting][Project Build TargetとLibrary][scale=0.40]{
//}

確認が必要な情報は次の通りです。

 * ［Project Build Target］で選択されているAndroidのバージョン（API Level）
 * ［Library］ → ［is library］のチェックの有無
 * ［Reference］に登録されているプロジェクト名

例となる「farewelladt_workspace」には2つのプロジェクト「FarewellAdt」と「library」があります。
それぞれについて値を確認していきます。

//table[migration_checksheet1][FarewellAdtプロジェクト]{
確認項目	値
---------------
プロジェクト名	FarewellAdt
パッケージ名	io.keiji.farewelladt
バージョンコード	1
バージョン名	1.0
対応API Level	15
対象API Level	23
Project Build Target	Android 6.0 - API Level 23
ライブラリプロジェクト	false
依存するプロジェクト	library
利用ライブラリ	android-support-v4.jar
ユニットテスト	なし
NDK	あり
//}

//table[migration_checksheet2][libraryプロジェクト]{
確認項目	値
---------------
プロジェクト名	library
パッケージ名	io.keiji.library
バージョンコード	1
バージョン名	1.0
対応API Level	8
対象API Level	21
Project Build Target	Android 6.0 - API Level 23
ライブラリプロジェクト	true
依存するプロジェクト	なし
利用ライブラリ	android-support-v4.jar
ユニットテスト	なし
NDK	なし
//}

== 手動での移行（実践編）
情報が揃ったら、いよいよ移行作業を始めます。
移行作業は次のステップで進めていきます。

 1. 新規（Android Studio）プロジェクトの作成
 2. モジュールの作成
 3. ライブラリの移行
 4. リソースの移行
 5. アセットの移行
 6. ソースコードの移行
 7. テストの移行
 8. AndroidManifest.xmlの移行
 9. NDK（JNI）の移行

移行の途中でエラーが出ても慌てないでください。
エラーの修正は一通り移行作業が終わった後の方が負担が少ないので、まずは最後までやりきるように心がけましょう。

=== 新規プロジェクトの作成
移行先（Android Studio）でプロジェクトを作成します。
まずはアプリの起点となる「FarewellAdt」から作成しましょう。

初期の画面から［Start a new Android Studio project］をクリックすると、プロジェクトウィザードが起動します。

==== Company DomainとPackage name
［Application name］と［Company Domain］をそれぞれ入力します。
これらを組み合わせたものがデフォルトの［Package name］になります。

//image[as_project_wizard1][][scale=0.35]{
//}

この際［Company Domain］は自動的に逆順になります。
ADTのプロジェクトウィザードのように逆順で入力してしまうと、間違った［Package name］が設定されてしまうので注意が必要です。

なお、［Edit］をクリックすると、［Package name］を直接書き換えることができます。

==== 対応バージョン
対応バージョンを選択します。ここで選択したAPI Levelが、@<tt>{minSdkVersion}になります。
後から書き換えることもできますが、今回は先ほど確認した値「Android 4.0.3 (Ice Cream Sandwich)」を選択します。

//image[as_project_wizard2][][scale=0.35]{
//}

最後に、追加するActivityを選択する画面が表示されますが、今回はADTからの移行なのでActivityは必要ありません。

［Add no Activity］を選択して［Finish］をクリックすると、Android Studioはプロジェクトの生成を開始します。

//image[as_project_wizard3][][scale=0.35]{
//}

Android Studioでは、プロジェクトの生成時にはインターネット接続が必要となる場合があります。
プロジェクトの生成（ビルド）時に必要なソフトウェアをインターネットを通じてダウンロードするためです。


====[column] 管理の単位の違い

ADTとAndroid Studioでは「プロジェクト」という単語の意味が異なるので注意が必要です。

ADT（Eclipse）における最上位の単位は「ワークスペース」と呼ばれ、ワークスペースの下には複数の「プロジェクト」が配置されます。
一方、Android Studio（IntelliJ IDEA）は「プロジェクト」が最上位の単位であり、その下に「モジュール」が配置される構造になっています。

====[/column]

==== Project Viewへの切り替え
Android Studioは、プロジェクトの作成直後には標準で「Android View」を表示します。
しかし「Android View」は、実際のプロジェクト構造が見えず、移行作業には適しません。

作業を進めるにあたってまず、左上のメニューから「Project View」に切り替えてください。

//image[project_view][［Project］を選択する。［Project Files］でないことに注意][scale=0.6]{
//}


=== モジュールの作成
次に、モジュールを作成します。
ADTでは「library」として扱っていたプロジェクトを「モジュール」にします。

［File］メニューから［New］→［New Module］をクリックすると、モジュール作成ウィザードが起動します。

まず、モジュールの種類を選択します。ADTの「library」プロジェクトの場合は、［Android Library］を選択します
（ADTのプロジェクトがアプリケーションなら、［Phone and Tablet］を選択します）。

//image[create_module1][][scale=0.35]{
//}

［Application/Library name］を入力@<fn>{capital}し、次に対応バージョンを選択します。
ここで選択したAPI Levelが、モジュールの@<tt>{minSdkVersion}になります。
ADTの「library」プロジェクトの場合は、「Android 2.2 (Froyo)」を選択します。

//footnote[capital][［Application/Library name］は、先頭が大文字で入力することが推奨されていますが、ADTのプロジェクトに完全に合わせるなら小文字でも問題ありません]

//image[create_module2][][scale=0.35]{
//}

［Finish］をクリックすると、Android Studioはモジュール「library」を生成します。

==== 参照（dependencies）の追加
ここまでを終えて、Android Studioのプロジェクトには「FarewellAdt」と「library」の2つのモジュールがあります。

しかし、この段階では「FarewellAdt」は「library」にあるコードやリソースを参照できません。
モジュール「FarewellAdt」から、モジュール「library」へ参照を設定する必要があります。

参照は、@<tt>{build.gradle}に記述します。
モジュール「FarewellAdt」の@<tt>{app/build.gradle}を開いて、@<list>{add_dependencies}のように@<tt>{dependencies}に@<tt>{compile project(’:library’)}を追記します。

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

//indepimage[gradlephant][Gradlephant][scale=0.6]

ADTでは、Eclipseとプラグインがビルドを実行して@<kw>{APK, Application PacKage}を生成しています。
また、@<kw>{CI, Continuous Integration}を実施しようとすれば、@<tt>{android}コマンドを使ってAntのビルドファイル生成するのが一般的です。

しかし、ADTとAntはあくまで別のビルド環境です。
Antのビルド設定を複雑にしていくうちに、ADTではビルドできないプロジェクト構造になる（分断されてしまう）ことも珍しくありません。
それでは、統合開発環境としてのADTの機能がフルに発揮できなくなります。

一方、「Gradle」を使ってビルドするAndroid Studioは、ADTとAntのような分断が起こりません。

Gradleによるビルドは、ADTからAndroid Studioに移行する最も大きなメリットと言えるでしょう。

====[/column]

=== ライブラリの移行
続いて、ライブラリを移行します。

ADTのプロジェクト「FarewellAdt」と「library」は、どちらもライブラリ@<tt>{android-support-v4.jar}を@<tt>{libs}に配置しています。

Android Studioのモジュール「FarewellAdt」と「library」の@<tt>{build.gradle}を開くと、どちらも@<tt>{dependencies}に@<tt>{appcompat-v7}が設定されています
（@<list>{farewelladt_appcompat}）。

@<tt>{appcompat-v7}は、@<tt>{android-support-v4}を含みます@<fn>{compat-support}。
つまり、このケースではライブラリの移行は必要ないということになります。

//footnote[compat-support][Gradleでappcompat-v7を設定すると、自動的にandroid-support-v4の機能も取り込まれます]

//list[farewelladt_appcompat][appcompat-v7への参照を設定している]{
dependencies {
    compile fileTree(dir: 'libs', include: ['*.jar'])
    ...
    compile 'com.android.support:appcompat-v7:23.1.1'
    ...
}
//}

==== リポジトリからライブラリを追加
Android Studio（Gradle）では基本的に、ライブラリの追加にあたってjarファイルを@<tt>{libs}に配置する必要がありません。
使いたいライブラリの名前とバージョンをdependenciesに記載すると、Gradleは、Maven Cnetral@<fn>{url_maven}やjCenter@<fn>{url_jcenter}などのリポジトリからバイナリを自動的にダウンロードしてビルドを実行します。

例えば、ButterKnifeを使う場合、ADTではButterKnifeのバイナリ（jar）ファイルをダウンロードして@<tt>{libs}にコピーします。
一方、Android Studio（Gradle）では、@<tt>{dependencies}にButterKnifeの情報を記載するだけで設定が完了します（@<list>{add_butterknife}）。

もちろん、リポジトリに登録されていないライブラリについては、これまで通りjarファイルを@<tt>{libs}に配置できます。

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
ADTのプロジェクトのリソースは、@<tt>{res}ディレクトリにあります。
Android Studioの各モジュールの@<tt>{src/main/res}ディレクトリにコピーします。

====[column] drawableとmipmap

ADTでは、アプリのアイコンは標準で@<tt>{res/drawable-*}に配置されます。

一方、Android Studioでは、アプリのアイコンは標準で@<tt>{src/main/res/mipmap-*}に配置されます。

mipmapは、3Dテクスチャとしての描画に最適化された画像群の名称ですが、実際にはこれまで@<tt>{res/drawable-*}に置いていた画像と違いはありません。
PNG形式で、各解像度別の画像サイズも同じです。

しかし、@<tt>{src/main/res/mipmap-*}に配置したファイルは@<tt>{R.mipmap.*}や@<tt>{@mipmap/*}を通じてアクセスすることに注意してください。
ソースコードやリソースから@<tt>{drawable}として参照していた画像を@<tt>{mipmap}に置く場合、それぞれの参照を変更しなくてはなりません。

ADTから画像リソースを移行する場合、アイコン画像は@<tt>{src/main/res/mipmap-*}に移動する。
また@<tt>{src/main/AndroidManifest.xml}を開いて、アイコンの参照先を@<tt>{@drawable/ic_launcher}から@<tt>{@mipmap/ic_launcher}に変更するなどして調整してください。

====[/column]

=== アセットの移行
ADTのプロジェクトのアセットは、@<tt>{assets}ディレクトリにあります。
Android Studioの各モジュールの@<tt>{src/main/assets}ディレクトリにコピーします。

=== Javaソースコードの移行
ADTのプロジェクトのJavaソースコードは、@<tt>{src}ディレクトリにあります。
Android Studioの各モジュールの@<tt>{src/main/java}ディレクトリにコピーします。

この際、ADTからパッケージを選択してコピー&ペーストすると、パッケージ構造を正確にコピーできない場合があります。
エクスプローラー（OS Xの場合はFinder）などを使ってディレクトリ（パッケージ）ごとコピーしてください。

=== テストコードの移行
ADTにテストコードがある場合、Android Studioの各モジュールの@<tt>{src/AndroidTest/java}ディレクトリにコピーします
（Android Studioは、テストコードをモジュール毎に管理します）。

この際、ADTからパッケージを選択してコピー&ペーストすると、パッケージ構造を正確にコピーできない場合があります。
エクスプローラー（OS Xの場合はFinder）などを使ってディレクトリ（パッケージ）ごとコピーしてください。

==== build.gradleの設定
最後に、テストを実行するために、@<tt>{build.gradle}に@<tt>{testInstrumentationRunner}を記述します。

また、@<tt>{dependencies}に@<tt>{com.android.support.test:support-annotations}と@<tt>{com.android.support.test:runner}を追加します。

//list[build_gradle_for_test][testInstrumentationRunnerとdependenciesを記述する]{
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

        // 実際には一行で記述
        testInstrumentationRunner "android.support.test.runner.AndroidJUnitRunn\
er"
    }

    buildTypes {
        release {
            minifyEnabled false
            proguardFiles getDefaultProguardFile('proguard-android.txt'),
                    'proguard-rules.pro'
        }
    }
}
dependencies {
    compile fileTree(include: ['*.jar'], dir: 'libs')
    testCompile 'junit:junit:4.12'
    compile 'com.android.support:appcompat-v7:23.1.1'
    compile project(':library')

    androidTestCompile 'com.android.support:support-annotations:23.1.1'
    androidTestCompile 'com.android.support.test:runner:0.3'
}
//}

=== AndroidManifest.xmlの移行
ADTのプロジェクトの@<tt>{AndroidManifest.xml}は、プロジェクトの直下にあります。
Android Studioの各モジュールの@<tt>{src/main/}ディレクトリにある@<tt>{AndroidManifest.xml}に内容をコピーします。

==== build.gradleへ項目を移動する
Android Studioでは、ADTで@<tt>{AndroidManifest.xml}に設定していた値のいくつかは、@<tt>{build.gradle}に設定するように変更されています。

@<tt>{build.gradle}へ移動する項目を@<table>{move_to_buildgradle}に示します。

//table[move_to_buildgradle][build.gradleへ移動する項目]{
作業項目
---------------
compileSdkVersion
minSdkVersion
targetSdkVersion
versionCode
versionName
//}

これらの値を@<tt>{AndroidManifest.xml}（@<list>{adt_android_manifest}）から@<tt>{build.gradle}に移動します。

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

次に、@<tt>{versionCode}と@<tt>{versionName}の値を@<tt>{build.gralde}に移行します（@<list>{as_buildgradle}）。

//list[as_buildgradle][versionCodeとversionNameの値を設定]{
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
            proguardFiles getDefaultProguardFile('proguard-android.txt'),
                    'proguard-rules.pro'
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

最後に、移行した各値をAndroid Studio側の@<tt>{AndroidManifest.xml}から削除すれば移行は完了です。

=== NDKの移行
@<kw>{NDK,Native Development Kit}を使っている場合は、ネイティブのコードを移行するのに加え、NDKを扱えるように設定する必要があります。

==== NDKのパスを設定
Android StudioにNDKのパスを設定します。
プロジェクトで右クリックをして［Open Module Settings］をクリックすると設定画面が開きます。

［SDK Locations］の［Android NDK Location］に、NDKのパスを設定します。

//image[ndk_setting][Android NDK Location][scale=0.35]{
//}

==== ネイティブのソースコード
ADTのプロジェクトのネイティブのソースコードは、@<tt>{jni}ディレクトリにあります。
Android Studioの各モジュールの@<tt>{src/main/jni}ディレクトリにコピーします。

==== build.gradleの設定
続いて、@<tt>{build.gradle}をNDK用に設定します。

まず、プロジェクトのトップレベルにある@<tt>{build.gradle}を変更します（@<list>{top_build_gradle}）。
本稿執筆時点で、NDKビルドに対応しているのは実験（experimental）バージョンのGradleだけです。

//list[top_build_gradle][プロジェクトのトップにあるbuild.gradle]{

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

====[column] gradle-experimentalのバージョン

本稿執筆時点（2015年12月）で@<tt>{gradle-experimental}の最新バージョンは@<tt>{0.6.0-alpha2}です。

@<tt>{0.6.0-alpha2}に設定した場合、@<tt>{gradle/wrapper/gradle-wrapper.properties}で@<tt>{gradle-2.9-all.zip}を指定する必要があります。

====[/column]

次に、プロジェクト「FarewellAdt」と「library」それぞれに適用するpluginを変更します。

@<list>{farewelladt_buildgradle}は、@<tt>{com.android.application}を@<tt>{com.android.model.application}に変更しています。

@<tt>{com.android.model.application}は通常のプラグインと@<kw>{DSL,Domain-Specific Language}が違うので注意してください。

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

====[column] 注意

ライブラリプロジェクトにJNIを設定する際、@<tt>{versionCode}と@<tt>{versionName}を削除するのを忘れないようにしてください。

ライブラリの場合、@<tt>{defaultConfig.with}の中に@<tt>{versionCode}や@<tt>{versionName}があると、@<tt>{org.gradle.api.internal.ExtensibleDynamicObject}が原因でビルドに失敗します。

その他、NDKに関する最新の情報、詳細な設定方法については次のサイトを参照してください。

 * 参考
 ** http://tools.android.com/tech-docs/new-build-system/gradle-experimental

====[/column]
