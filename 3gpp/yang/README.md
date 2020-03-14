# 3GPP 28541 and 28623

Sources are word documents. The specs have been manually copied and pasted:
* http://www.3gpp.org/ftp//Specs/archive/28_series/28.541/28541-g30.zip
* http://www.3gpp.org/ftp//Specs/archive/28_series/28.623/28623-g20.zip

Modifications:



diff --git a/3gpp/yang/_3gpp-5gc-nrm-ausffunction@2019-10-25.yang b/3gpp/yang/_3gpp-5gc-nrm-ausffunction@2019-10-25.yang
index 8f8e36f..81b1251 100644
--- a/3gpp/yang/_3gpp-5gc-nrm-ausffunction@2019-10-25.yang
+++ b/3gpp/yang/_3gpp-5gc-nrm-ausffunction@2019-10-25.yang
@@ -54,15 +54,13 @@ module _3gpp-5gc-nrm-ausffunction {
       min-elements 1;
       uses types3gpp:ManagedNFProfile;
     }
-  }
 
     list commModelList {
       min-elements 1;
       key "groupId";
       uses types5g3gpp:CommModel;
     }

diff --git a/3gpp/yang/_3gpp-5gc-nrm-ep@2019-11-18.yang b/3gpp/yang/_3gpp-5gc-nrm-ep@2019-11-18.yang
index 47822ef..d8d9237 100644
--- a/3gpp/yang/_3gpp-5gc-nrm-ep@2019-11-18.yang
+++ b/3gpp/yang/_3gpp-5gc-nrm-ep@2019-11-18.yang
@@ -2,9 +2,12 @@ module _3gpp-5gc-nrm-ep {
   yang-version 1.1;
   namespace "urn:3gpp:tsg:sa5:nrm:_3gpp-5gc-nrm-ep";
   prefix "cep3gpp";
-  
+
+  import ietf-inet-types { prefix inet; }
+
   import _3gpp-common-ep-rp { prefix eprp3gpp; }
   import _3gpp-common-managed-element { prefix me3gpp; }
+  import _3gpp-common-yang-types { prefix types3gpp; }
   import _3gpp-5gc-nrm-affunction { prefix af3gpp; }
   import _3gpp-5gc-nrm-amffunction { prefix amf3gpp; }
   import _3gpp-5gc-nrm-ausffunction { prefix ausf3gpp; }
@@ -478,6 +481,7 @@ module _3gpp-5gc-nrm-ep {
         uses EP_N32Grp;
       }
     }
+  }


diff --git a/3gpp/yang/_3gpp-5gc-nrm-smsffunction@2019-10-25.yang b/3gpp/yang/_3gpp-5gc-nrm-smsffunction@2019-10-25.yang
index 05ce031..8a6df72 100644
--- a/3gpp/yang/_3gpp-5gc-nrm-smsffunction@2019-10-25.yang
+++ b/3gpp/yang/_3gpp-5gc-nrm-smsffunction@2019-10-25.yang
@@ -17,7 +17,7 @@ module _3gpp-5gc-nrm-smsffunction {
     
   revision 2019-05-15 {
     description "initial revision";
-    reference "Based on
+    reference "Based on";
   }
   
   grouping SMSFFunctionGrp {

diff --git a/3gpp/yang/_3gpp-5gc-nrm-udsffunction@2019-10-25.yang b/3gpp/yang/_3gpp-5gc-nrm-udsffunction@2019-10-25.yang
index db7af38..af9ff23 100644
--- a/3gpp/yang/_3gpp-5gc-nrm-udsffunction@2019-10-25.yang
+++ b/3gpp/yang/_3gpp-5gc-nrm-udsffunction@2019-10-25.yang
@@ -18,7 +18,7 @@ module _3gpp-5gc-nrm-udsffunction {
   
   revision 2019-05-22 {
     description "initial revision";
-    reference "Based on
+    reference "Based on";
   }
   
   grouping UDSFFuntionGrp {

diff --git a/3gpp/yang/_3gpp-5gc-nrm-upffunction@2019-10-25.yang b/3gpp/yang/_3gpp-5gc-nrm-upffunction@2019-10-25.yang
index d1c2b19..f76952a 100644
--- a/3gpp/yang/_3gpp-5gc-nrm-upffunction@2019-10-25.yang
+++ b/3gpp/yang/_3gpp-5gc-nrm-upffunction@2019-10-25.yang
@@ -53,7 +53,7 @@ module _3gpp-5gc-nrm-upffunction {
       min-elements 1;
       uses types3gpp:ManagedNFProfile;
     }
-  }
+
     list commModelList {
       min-elements 1;
       key "groupId";

diff --git a/3gpp/yang/_3gpp-5gc-nrm-seppfunction@2019-11-17.yang b/3gpp/yang/_3gpp-5gc-nrm-seppfunction@2019-11-17.yang
index 3ed37ed..f780d3e 100644
--- a/3gpp/yang/_3gpp-5gc-nrm-seppfunction@2019-11-17.yang
+++ b/3gpp/yang/_3gpp-5gc-nrm-seppfunction@2019-11-17.yang
@@ -4,6 +4,8 @@ module _3gpp-5gc-nrm-seppfunction {
   namespace urn:3gpp:sa5:_3gpp-5gc-nrm-seppfunction;
   prefix sepp3gpp;
   
+  import ietf-inet-types { prefix inet; }
+
   import _3gpp-common-managed-function { prefix mf3gpp; }
   import _3gpp-common-managed-element { prefix me3gpp; }
   import _3gpp-common-yang-types { prefix types3gpp; }
@@ -14,11 +16,12 @@ module _3gpp-5gc-nrm-seppfunction {
                and policing on inter-PLMN control plane interface. For more information about the SEPP, see 3GPP TS 23.501.";
   reference "3GPP TS 28.541";
   
-  revision 2019-10-28 { reference S5-193518 ; }
-
   revision 2019-11-17 {
     description "initial revision";
   }
+  revision 2019-10-28 { reference S5-193518 ; }
+
+
   typedef SEPPType {


diff --git a/3gpp/yang/_3gpp-5gc-nrm-amffunction@2019-10-25.yang b/3gpp/yang/_3gpp-5gc-nrm-amffunction@2019-10-25.yang
index 37c2608..50ae13a 100644
--- a/3gpp/yang/_3gpp-5gc-nrm-amffunction@2019-10-25.yang
+++ b/3gpp/yang/_3gpp-5gc-nrm-amffunction@2019-10-25.yang
@@ -6,6 +6,7 @@ module _3gpp-5gc-nrm-amffunction {
   import _3gpp-common-managed-function { prefix mf3gpp; }
   import _3gpp-common-managed-element { prefix me3gpp; }
   import _3gpp-common-yang-types { prefix types3gpp; }
+  import _3gpp-5g-common-yang-types { prefix types5g3gpp; }
   import ietf-inet-types { prefix inet; }
   import _3gpp-common-top { prefix top3gpp; }


diff --git a/3gpp/yang/_3gpp-5gc-nrm-lmffunction@2019-10-25.yang b/3gpp/yang/_3gpp-5gc-nrm-lmffunction@2019-10-25.yang
index b7d87a0..9871ab3 100644
--- a/3gpp/yang/_3gpp-5gc-nrm-lmffunction@2019-10-25.yang
+++ b/3gpp/yang/_3gpp-5gc-nrm-lmffunction@2019-10-25.yang
@@ -7,6 +7,7 @@ module _3gpp-5gc-nrm-lmffunction {
   import _3gpp-common-managed-function { prefix mf3gpp; }
   import _3gpp-common-managed-element { prefix me3gpp; }
   import _3gpp-common-yang-types { prefix types3gpp; }
+  import _3gpp-5g-common-yang-types { prefix types5g3gpp; }
   import _3gpp-common-top { prefix top3gpp; }

mv _3gpp-5gc-nrm-ep@2019-05-31.yang _3gpp-5gc-nrm-ep@2019-11-18.yang
mv _3gpp-5gc-nrm-seppfunction@2019-05-15.yang  _3gpp-5gc-nrm-seppfunction@2019-10-28.yang

