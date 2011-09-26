# port/package comment, name, category, and maintainer.
COMMENT=                Proview development and runtime package
DISTNAME=               pwr48-4.8.3
CATEGORIES=             misc
MAINTAINER=             Proview <postmaster@proview.se>

# Mandatory licensing variables.
PERMIT_PACKAGE_CDROM=   NO
PERMIT_PACKAGE_FTP=     NO
PERMIT_DISTFILES_CDROM= NO
PERMIT_DISTFILES_FTP=   NO

# Don't build or run regress tests; specify all architectures.
NO_BUILD=               Yes
NO_REGRESS=             Yes
PKG_ARCH=               amd64

# Install under /usr/local/.
do-install:
	${INSTALL_SCRIPT} ${WRKSRC}/bin/* ${PREFIX}/bin/
	${INSTALL_DATA_DIR} ${PREFIX}/share/myscripts/
	${INSTALL_DATA} ${WRKSRC}/share/pwr48/* ${PREFIX}/share/pwr48/

# Include the BSD port system.
.include <bsd.port.mk>
