= Gradle 101

//lead{
この章では、ユースケース毎にGradleの設定方法を解説します。
//}

== 含めるソースコードやリソースを入れ替えたい
「productFlavors」を使えば、生成するAPKファイルに含めるソースコードやリソースを、ビルドのタスクに応じて入れ替えることができます。

ここでは無料の試用版（trial）と、有料版（commercial）を分ける場合を考えます。

=== flavorを設定する
まず、@<tt>{app/src}の下に２つのディレクトリ「trial」と「commercial」を作成します。

//cmd[commercialとtrialを追加した構成]{
.
|-- app.iml
|-- build
|-- build.gradle
|-- libs
|-- proguard-rules.pro
`-- src
    |-- androidTest
    |-- commercial
    |-- main
    |   |-- AndroidManifest.xml
    |   |-- java.io.keiji.farewelladt
    |   |   `-- MainActivity.java
    |   `-- res
    |-- test
    `-- trial
//}

次に、@<tt>{app/build.gradle}を開いて、productFlavorsの情報を加えると、準備は完了です（@<list>{add_product_flavors}）。

//list[add_product_flavors][]{
apply plugin: 'com.android.application'

android {

    // 省略

    productFlavors {
        trial
        commercial
    }
}
//}

=== クラスを入れ替える
@<tt>{trial}と@<tt>{commercial}それぞれに、切り替えたいクラス（e.g. @<tt>{FooBar}）を配置します。

//cmd[FooBar.javaをそれぞれのflavorに配置する]{
.
|-- app.iml
|-- build
|-- build.gradle
|-- libs
|-- proguard-rules.pro
`-- src
    |-- androidTest
    |-- commercial
    |   `-- java.io.keiji.farewelladt
    |       `-- FooBar.java
    |-- main
    |   |-- AndroidManifest.xml
    |   |-- java.io.keiji.farewelladt
    |   |   `-- MainActivity.java
    |   `-- res
    |-- test
    `-- trial
        `-- java.io.keiji.farewelladt
            `-- FooBar.java
//}

この状態でflavorをビルドすれば、２つのflavorはそれぞれの@<tt>{FooBar.java}が組み込まれた状態でAPKが作成されます。

====[column] クラスの重複（mainとflavor）

flavorの下に置いたクラスを、@<tt>{main}には置かないように注意して下さい。
また、flavorの下に置いたクラスで@<tt>{main}から参照するメソッド、フィールドはすべて共通にして置く必要があります。
mainとクラスが重複した場合や、メソッド、フィールドが参照できない場合、エラーが起きてビルドが完了しません。

====[/column]

Android Studioの「Gradle」タブで表示されるタスク一覧から@<tt>{assemble}を実行すれば、すべてのflavorをビルドできます。
さらに@<tt>{assembleTrial}や@<tt>{assembleCommercial}のように、flavorを個別にビルドすることもできます。

//image[as_gradle_tasks][タスク一覧][scale=0.4]{
//}

アプリをエミュレーターや実機で実行するflavorを選びたい場合、Android Studioの「Build Variants」タブで切り替えることができます。

なお「Build Variants」とは、ここで述べた「productFlavors」と、デバッグ版・リリース版を切り替える「buildTypes」の２つの要素を合わせたものです。

//image[as_build_variants][Build Variants][scale=0.6]{
//}

コマンドラインからもビルドを実行できます。
エクスプローラー（OS Xの場合はFinder）からプロジェクトのトップにディレクトリを移動して、@<tt>{./gradlew [タスク名]}を実行します。
@<tt>{./gradlew tasks}を実行すれば、利用できるタスク一覧を表示できます。

//cmd[]{
$ ./gradlew tasks
Starting a new Gradle Daemon for this build (subsequent builds will be faster).
Parallel execution is an incubating feature.
:tasks

------------------------------------------------------------
All tasks runnable from root project
------------------------------------------------------------

Android tasks
-------------
androidDependencies - Displays the Android dependencies of the project.
signingReport - Displays the signing info for each variant.
sourceSets - Prints out all the source sets defined in this project.

Build tasks
-----------
assemble - Assembles all variants of all applications and secondary packages.
assembleAndroidTest - Assembles all the Test applications.
assembleCommercial - Assembles all Commercial builds.
assembleDebug - Assembles all Debug builds.
assembleRelease - Assembles all Release builds.
assembleTrial - Assembles all Trial builds.
build - Assembles and tests this project.
buildDependents - Assembles and tests this project and all projects that depend on it.
buildNeeded - Assembles and tests this project and all projects it depends on.
clean - Deletes the build directory.

[省略]

BUILD SUCCESSFUL

Total time: 8.705 secs
//}

=== リソースを入れ替える
@<tt>{commercial}に、切り替えたいリソース（e.g. @<tt>{ic_launcher.png}）を配置します。

//cmd[commercialのflavorにだけ特別なアイコンを配置する]{
.
|-- app.iml
|-- build.gradle
|-- libs
|-- proguard-rules.pro
`-- src
    |-- androidTest
    |-- commercial
    |   `-- res
    |       `-- mipmap-xxxhdpi
    |           `-- ic_launcher.png
    |-- main
    |   |-- AndroidManifest.xml
    |   |-- java
    |   `-- res
    |       |-- drawable
    |       |-- layout
    |       |-- mipmap-xxxhdpi
    |       |   `-- ic_launcher.png
    |       |-- values
    |       `-- values-w820dp
    |-- test
    `-- trial
//}

この状態でflavorをビルドすれば、@<tt>{commercial}はflavorに設定したアイコンが組み込まれた状態でAPKが作成されます。

====[column] リソースの重複（mainとflavor）

リソースの場合は@<tt>{main}のリソースと重複しても、ビルドエラーにはなりません。
flavorにリソースがあればflavorのものが優先され、なかった場合は@<tt>{main}のリソースがAPKに組み込まれます。

====[/column]

== ビルドによってアプリケーションの情報を変えたい
Build Variantsに設定することで、アプリのさまざまな情報（applicationId, minSdkVersion, versionCode, versionName）をビルドに応じて変えることができます。

=== applicationIdSuffixとversionNameSuffixの付加
@<list>{ApplicationIdSuffix}を設定すれば、ビルドするアプリの基本となるapplicationId（defaultConfigのもの）の末尾に、接尾辞（Suffix）を付加できます。

//list[ApplicationIdSuffix][debugビルドでapplicationIdの末尾に.debugを付加する]{
android {
    defaultConfig {
        applicationId "io.keiji.farewelladt"
        minSdkVersion 15
        targetSdkVersion 23
        versionCode 1
        versionName "1.0"
    }

    buildTypes {
        debug {
            applicationIdSuffix '.debug'
            versionNameSuffix ' debug'
        }
        release {
            minifyEnabled true
            proguardFiles getDefaultProguardFile('proguard-android.txt'), 'proguard-rules.pro'

            buildConfigField "String", "URL_ENDPOINT", "\"https://blog.keiji.io/\""
        }
    }
}
//}

=== ApplicationIdの変更
Product Flavorsをつかうと、ビルドするアプリの@<tt>{applicationId}を変更できます。

@<list>{application_id}では、ビルドするアプリの基本となるapplicationIdそのものを変更しています。

//list[application_id][applicationId]{
android {
    defaultConfig {
        applicationId "io.keiji.farewelladt"
        minSdkVersion 15
        targetSdkVersion 23
        versionCode 1
        versionName "1.0"
    }

    productFlavors {
        trial {
        }
        commercial {
            applicationId 'io.keiji.helloandroidstudio.commercial'
        }
    }
}
//}

=== versionCodeおよびversionNameの変更
Product Flavorsをつかうと、ビルドするアプリの@<tt>{versionCode}および@<tt>{versionName}を変更できます。

@<list>{versioncode_and_version_name}では、ビルドするアプリの基本となるversionCodeとversionNameそのものを変更しています。

//list[versioncode_and_version_name][trialとcommercialで異なるversionCodeとversionNameを設定している]{
android {
    defaultConfig {
        applicationId "io.keiji.farewelladt"
        minSdkVersion 15
        targetSdkVersion 23
        versionCode 1
        versionName "1.0"
    }

    productFlavors {
        trial {
            versionCode 30
        }
        commercial {
            versionName 'Commercial Version'
            versionCode 10001
        }
    }
}
//}

=== AndroidManifest.xmlへの反映
applicationIdSuffixを付加したときやapplicationIdそのものを変えた場合、システムに同じアプリを複数（デバッグ版とリリース版など）のアプリをインストールできます。

しかし、@<tt>{AndroidManifest.xml}に記述する項目についてはそのままでは変更されません。
すると、例えapplicationIdが違っていても、ContentProviderのauthoritiesが重複してアプリがインストールができないという問題が発生します。

AndroidManifest.xml側での関連部分を書き換えれば、変更したapplicationIdを反映できます。
この処理は@<tt>{Manifest Merger}が行います。

//list[androidmanifest][providerのauthoritiesをapplicationIdで置き換える]{
<?xml version="1.0" encoding="utf-8"?>
<manifest
    package="io.keiji.farewelladt"
    xmlns:android="http://schemas.android.com/apk/res/android">

    <application
        android:allowBackup="true"
        android:icon="@mipmap/ic_launcher"
        android:label="@string/app_name"
        android:supportsRtl="true"
        android:theme="@style/AppTheme">
        <activity android:name=".MainActivity">
            <intent-filter>
                <action android:name="android.intent.action.MAIN"/>

                <category android:name="android.intent.category.LAUNCHER"/>
            </intent-filter>
        </activity>

        <provider
            android:name=".provider.SomeContentProvider"
            android:authorities="${applicationId}"
            android:exported="false"/>
    </application>

</manifest>
//}

@<tt>{{applicationId}}としたところは、ビルド時にそのアプリのapplicationIdで置き換えられます。

そのほか、build.gradleとAndroidManifest.xmlとの連携については、次のURLを参照してください。

 * Android Tools Project Site - Manifest Merger
 ** http://tools.android.com/tech-docs/new-build-system/user-guide/manifest-merger

== 外部コマンドの実行結果をビルドに反映したい
ビルドの際に外部コマンドを実行できます。

たとえば、現在のプロジェクトをGitで管理していた場合、Gitのハッシュ値を取得する処理は@<list>{gitsha}のようになります。
@<code>{gitSha}メソッドの中で、実際に@<tt>{git}コマンドを実行しています。

さらに、buildTypesの@<tt>{debug}でメソッドを実行することで、versionNameの末尾にハッシュ値を追加しています（versionNameSuffix）。

//list[gitsha][gitSha]{
apply plugin: 'com.android.application'

def gitSha() {
    return 'git rev-parse --short HEAD'.execute().text.trim()
}

android {
    buildTypes {
        debug {
            applicationIdSuffix '.debug'
            versionNameSuffix ' ' + gitSha()
        }
    }
}
//}

== 新しいBuild Typeを追加したい
最初に定義されている@<tt>{debug}と@<tt>{release}以外にもBuidTypeを追加できます。

@<tt>{initWith}を使うと、すでにあるBuild Type設定を引き継いで新しいBuild Typeを追加することができます。

@<list>{add_buildtype_openbeta}は、@<tt>{debug}の設定を引き継いで、新しく@<tt>{openbeta}を作成する例です。
@<tt>{openbeta}をビルドすると、@<tt>{debug}のversionNameSuffixとapplicationIdSuffixの設定を引き継いだ上で、さらに@<tt>{minifyEnabled}を有効にした状態のAPKが生成されます。

//list[add_buildtype_openbeta][]{
    buildTypes {
        debug {
            applicationIdSuffix '.debug'
            versionNameSuffix ' ' + gitSha()
        }
        openbeta.initWith(buildTypes.debug)
        openbeta {
            minifyEnabled true
            proguardFiles getDefaultProguardFile('proguard-android.txt'), 'proguard-rules.pro'
        }
    }
//}

== ビルドによって定数の内容を変えたい
アプリのビルド時に自動で生成する@<tt>{BuildConfig}には、標準でいくつかの定数が宣言されています。

build.gradleから定数を新しく加えて、さらにBuild Variantsごとに変更できます（@<list>{build_config_field}）。

//list[build_config_field][buildConfigFieldによる定数の宣言]{
android {

    defaultConfig {
        applicationId "io.keiji.farewelladt"
        minSdkVersion 15
        targetSdkVersion 23
        versionCode 1
        versionName "1.0"

        buildConfigField "String", "URL_ENDPOINT", "\"https://test.keiji.io/\""
    }

    buildTypes {
        release {
            minifyEnabled true
            proguardFiles getDefaultProguardFile('proguard-android.txt'), 'proguard-rules.pro'

            buildConfigField "String", "URL_ENDPOINT", "\"https://blog.keiji.io/\""
        }
    }
}
//}

この状態でビルドをすると、@<list>{build_config_field_build_config}のように、新しく定数が追加されています。
また、リリースビルドでは変数の内容が指定したとおりに置き換えられます。

//list[build_config_field_build_config][定数URL_ENDPOINTが追加されている]{
/**
 * Automatically generated file. DO NOT MODIFY
 */
package io.keiji.farewelladt;

public final class BuildConfig {
  public static final boolean DEBUG = Boolean.parseBoolean("true");
  public static final String APPLICATION_ID = "io.keiji.farewelladt.debug";
  public static final String BUILD_TYPE = "debug";
  public static final String FLAVOR = "trial";
  public static final int VERSION_CODE = 30;
  public static final String VERSION_NAME = "1.0";
  // Fields from default config.
  public static final String URL_ENDPOINT = "https://test.keiji.io/";
}
//}

== Lintのエラーでリリースビルドを失敗させたくない
リリースビルドではLintによりコードがチェックされ、エラー項目があるとビルドを中止します。

@<list>{lint_options}を設定することで、Lintでエラーを指摘しても、リリースビルドを中止せず、リリース用のAPKを生成できます。

//list[lint_options][エラーで中止しないようにする]{
android {

    lintOptions {
        abortOnError false
    }
}
//}

===== 注意
Lintの指摘するエラーには修正して有益な項目が数多くあるので、この設定は慎重に行ってください。

== アプリの署名に関する情報をバージョン管理に含めたくない
アプリの署名に関する情報をbuild.gradleに記述することが、セキュリティ上、問題があることは言うまでもありません。

署名に関する情報をbuild.gradleから切り離すには、まず、新しく@<tt>{another-gradle.properties}@<fn>{filename_doesnt_matter}をプロジェクトのトップに作成します。
次に、@<tt>{another-gradle.properties}を@<tt>{.gitignore}に加えて、Gitの管理から外しておきます。

//footnote[filename_doesnt_matter][ファイル名に制約はありません]

そして、@<list>{another_properties}のようにキーストアの情報を記述します。

//list[another_properties][another-gradle.properties]{
storeFile=[キーストアのパス（フルパス）]
storePassword=[キーストアのパスワード]
keyAlias=[キーの名前]
keyPassword=[キーのパスワード]
//}

記述したら、次は@<list>{new_properties_buildgradle}のように@<tt>{build.gradle}を書き換えます。

signingConfigsの中で@<tt>{another-gradle.properties}があれば、ファイルをPropertiesとして読み込み、署名の情報として設定します。

//list[new_properties_buildgradle][]{
apply plugin: 'com.android.application'

android {

    signingConfigs {
        release {
            File propFile = rootProject.file("another-gradle.properties")
            if (propFile.exists()) {
                Properties props = new Properties()
                props.load(new FileInputStream(propFile))

                storeFile file(props.storeFile)
                storePassword props.storePassword
                keyAlias props.keyAlias
                keyPassword props.keyPassword
            }
        }
    }

    buildTypes {
        release {
            minifyEnabled false
            proguardFiles getDefaultProguardFile('proguard-android.txt'), 'proguard-rules.pro'

            signingConfig signingConfigs.release
        }
    }
}
//}
