pwr_top : build_pwr



build_pwr :
	@ if ./build.sh; then \
            echo "Proview built"; \
          fi
