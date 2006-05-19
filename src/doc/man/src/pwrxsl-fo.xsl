<?xml version='1.0'?>
<xsl:stylesheet  
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
    xmlns:fo="http://www.w3.org/1999/XSL/Format"
    version="1.0">

<!-- Import basic fo stylesheet-->
<xsl:import href="/usr/local/share/xml/docbook/stylesheet/snapshot/fo/docbook.xsl"/>

<!-- Import customized titlepage stylesheet-->
<xsl:include href="pwrtitlepages.xsl"/>

<!-- Include common-customization stylesheet -->
<xsl:include href="pwrxsl-common.xsl" />

<!-- Set fo specific parameters -->
<xsl:param name="paper.type" select="'A4'"/> 
<xsl:param name="preferred.mediaobject.role" select="'fo'"/>
<xsl:param name="fop1.extensions" select="1"/>
<xsl:param name="double.sided" select="0"></xsl:param>
<xsl:param name="draft.mode" select="'maybe'"></xsl:param>
<xsl:param name="draft.watermark.image" select="'http://docbook.sourceforge.net/release/images/draft.png'"></xsl:param>


<!-- Section title font sizes -->
<xsl:attribute-set name="section.title.level1.properties">
  <xsl:attribute name="font-size">
    <xsl:value-of select="$body.font.master * 1.6"/>
    <xsl:text>pt</xsl:text>
  </xsl:attribute>
</xsl:attribute-set>
<xsl:attribute-set name="section.title.level2.properties">
  <xsl:attribute name="font-size">
    <xsl:value-of select="$body.font.master * 1.3"/>
    <xsl:text>pt</xsl:text>
  </xsl:attribute>
</xsl:attribute-set>
<xsl:attribute-set name="section.title.level3.properties">
  <xsl:attribute name="font-size">
    <xsl:value-of select="$body.font.master * 1.2"/>
    <xsl:text>pt</xsl:text>
  </xsl:attribute>
</xsl:attribute-set>
<xsl:attribute-set name="section.title.level4.properties">
  <xsl:attribute name="font-size">
    <xsl:value-of select="$body.font.master * 1.1"/>
    <xsl:text>pt</xsl:text>
  </xsl:attribute>
</xsl:attribute-set> 


<!-- Graphics-related  -->

<xsl:template match="*" mode="admon.graphic.width">
  <xsl:text>24pt</xsl:text>
</xsl:template>


<xsl:param name="img.src.path">../src/</xsl:param>

<xsl:param name="callout.graphics" select="1"></xsl:param>
<xsl:param name="callout.graphics.path">../src/</xsl:param>
<xsl:param name="callout.graphics.extension" select="'.png'"></xsl:param>

<xsl:param name="admon.graphics" select="1"/>
<xsl:param name="admon.graphics.path">../src/</xsl:param>
<xsl:param name="admon.graphics.extension" select="'.png'"/>



<!-- fo formatting. For html output, this is done in the css-->
<xsl:template match="application[@role = 'pwrenv']">
  <fo:inline font-family="sans-serif">
    <xsl:apply-templates/>
  </fo:inline>
</xsl:template>

<xsl:template match="classname">
  <fo:inline font-weight="bold">
    <xsl:apply-templates/>
  </fo:inline>
</xsl:template>

<xsl:template match="property">
  <fo:inline font-style="italic">
    <xsl:apply-templates/>
  </fo:inline>
</xsl:template>


<!-- slightly tweaked version of menuchoice template, see change comment below -->

<xsl:template name="process.menuchoice">
  <xsl:param name="nodelist" select="guibutton|guiicon|guilabel|guimenu|guimenuitem|guisubmenu|interface"/><!-- not(shortcut) -->
  <xsl:param name="count" select="1"/>

  <xsl:variable name="mm.separator">
    <xsl:choose>
      <xsl:when test="($fop.extensions != 0 or $fop1.extensions != 0) and  
                contains($menuchoice.menu.separator, '&#x2192;')"> <!-- or fop1 added here -->
        <fo:inline font-family="Symbol">
          <xsl:copy-of select="$menuchoice.menu.separator"/>
        </fo:inline>
      </xsl:when>
      <xsl:otherwise>
        <xsl:copy-of select="$menuchoice.menu.separator"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:variable>

  <xsl:choose>
    <xsl:when test="$count>count($nodelist)"></xsl:when>
    <xsl:when test="$count=1">
      <xsl:apply-templates select="$nodelist[$count=position()]"/>
      <xsl:call-template name="process.menuchoice">
        <xsl:with-param name="nodelist" select="$nodelist"/>
        <xsl:with-param name="count" select="$count+1"/>
      </xsl:call-template>
    </xsl:when>
    <xsl:otherwise>
      <xsl:variable name="node" select="$nodelist[$count=position()]"/>
      <xsl:choose>
        <xsl:when test="name($node)='guimenuitem'
                        or name($node)='guisubmenu'">
          <xsl:copy-of select="$mm.separator"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:copy-of select="$menuchoice.separator"/>
        </xsl:otherwise>
      </xsl:choose>
      <xsl:apply-templates select="$node"/>
      <xsl:call-template name="process.menuchoice">
        <xsl:with-param name="nodelist" select="$nodelist"/>
        <xsl:with-param name="count" select="$count+1"/>
      </xsl:call-template>
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>


</xsl:stylesheet>