<?xml version='1.0'?>
<xsl:stylesheet  
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">

<!-- Import basic html stylesheet  
     This URI is not a web location, but is directed to the local stylesheet directory 
     (e.g /usr/local/share/xml/docbook/stylesheet/docbook-xsl-1.70.1)
     by means of the catalog.xml defined in the user's XML_CATALOG_FILES environment variable. -->
<xsl:import href="http://docbook.sourceforge.net/release/xsl/current/html/chunk.xsl"/>

<!-- Include common-customization stylesheet -->
<xsl:include href="pwrxsl-common.xsl" />
<xsl:include href="pwrxsl-html-common.xsl" />


<!--<xsl:param name="base.dir" select="'chunkoutput/'"></xsl:param>--> <!-- Define chunk output directory -->
<!--
<xsl:param name="chunk.fast" select="1"></xsl:param>
-->
<xsl:param name="use.id.as.filename" select="1"/>
<xsl:param name="chunk.section.depth" select="1"></xsl:param>
<xsl:param name="chunk.first.sections" select="1"></xsl:param>

<xsl:param name="suppress.navigation" select="0"></xsl:param>
<xsl:param name="suppress.header.navigation" select="0"></xsl:param>
<xsl:param name="suppress.footer.navigation" select="0"></xsl:param>
<xsl:param name="header.rule" select="1"></xsl:param>
<xsl:param name="footer.rule" select="1"></xsl:param>

</xsl:stylesheet>